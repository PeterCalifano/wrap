#!/usr/bin/env python3
"""Standalone C-vs-C++ MEX backend benchmark runner.

Generates, compiles and runs the runtime_demo wrapper for both MEX APIs ('c' and
'cpp'), scrapes the BENCH lines emitted by run_benchmark.m, and records each
backend's static footprint (.mex size + linked libs). Prints timing and memory
comparison tables; with --out-dir, also writes results.csv and results.md.

Usage:
    python tests/matlab_runtime/run_benchmark.py \
        --iters 2000 --sizes 8,64,256,1024 --time-v --out-dir /tmp/bench

Requires MATLAB (mex), gtsam headers/libs. See
docs/development/c-vs-cpp-mex-benchmark.md.
"""
# pylint: disable=import-error, wrong-import-position

import argparse
import csv
import glob
import os
import os.path as osp
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, osp.dirname(osp.abspath(__file__)))
import _harness  # noqa: E402

APIS = ("c", "cpp")
BENCH_RE = re.compile(r"^BENCH,([^,]+),([^,]+),([^,]+),([^,]+),(.+)$")
# Libs whose presence distinguishes the two artifacts.
WATCH_LIBS = ("libMatlabDataArray", "libmex", "libmx")


def _parse_bench(output):
    """Return {(op, size): value_str} timing rows and {metric: value} memory."""
    timing, memory = {}, {}
    for line in output.splitlines():
        m = BENCH_RE.match(line.strip())
        if not m:
            continue
        op, _api, _reps, size, value = m.groups()
        if op.startswith("vmrss_") or op.startswith("vmhwm_"):
            memory[op] = value
        else:
            timing[(op, int(size))] = value
    return timing, memory


def _ldd_watch(mex_path):
    """Return the sorted subset of WATCH_LIBS that `mex_path` links against."""
    try:
        out = subprocess.run(["ldd", mex_path], capture_output=True,
                             text=True, check=False).stdout
    except OSError:
        return []
    return sorted(lib for lib in WATCH_LIBS if lib in out)


def run_one(api, iters, sizes, time_v):
    """Build and run one backend. Returns a result dict."""
    out_dir = tempfile.mkdtemp(prefix="gtwrap_bench_{}_".format(api))
    try:
        _harness.generate(out_dir, api)
        mex_path = _harness.compile_wrapper(out_dir, api)
        size_list = "[" + " ".join(str(s) for s in sizes) + "]"
        call = "run_benchmark('{}','{}',{},{})".format(
            out_dir, api, iters, size_list)
        result = _harness.run_matlab(
            call, expect="ALL BENCHMARKS COMPLETE", time_v=time_v, timeout=1800)
        if result.returncode != 0:
            raise RuntimeError(
                "MATLAB benchmark ({}) exited non-zero:\n{}".format(api, result.output))
        timing, memory = _parse_bench(result.output)
        return {
            "api": api,
            "timing": timing,
            "memory": memory,
            "mex_bytes": os.path.getsize(mex_path),
            "libs": _ldd_watch(mex_path),
            "peak_rss_kb": result.peak_rss_kb,
        }
    finally:
        # Keep the temp dir only if the user asked to inspect artifacts.
        import shutil
        shutil.rmtree(out_dir, ignore_errors=True)


def _ratio(a, b):
    try:
        return "{:.2f}x".format(float(a) / float(b)) if float(b) else "n/a"
    except (ValueError, ZeroDivisionError):
        return "n/a"


def _timing_table(results, sizes):
    """List of [op, size, c_us, cpp_us, c/cpp ratio] rows."""
    c, cpp = results["c"]["timing"], results["cpp"]["timing"]
    rows = []
    keys = sorted(set(c) | set(cpp), key=lambda k: (k[0], k[1]))
    for op, size in keys:
        cv, pv = c.get((op, size), "NA"), cpp.get((op, size), "NA")
        rows.append([op, size, cv, pv, _ratio(cv, pv)])
    return rows


def _memory_rows(results):
    c, cpp = results["c"]["memory"], results["cpp"]["memory"]
    rows = []
    for metric in sorted(set(c) | set(cpp)):
        rows.append([metric, c.get(metric, "NA"), cpp.get(metric, "NA")])
    return rows


def _print_report(results, sizes):
    print("\n=== Timing (median us/call; ratio = C / C++, >1 means C slower) ===")
    print("{:<18}{:>8}{:>14}{:>14}{:>10}".format(
        "op", "size", "c_us", "cpp_us", "c/cpp"))
    for op, size, cv, pv, ratio in _timing_table(results, sizes):
        print("{:<18}{:>8}{:>14}{:>14}{:>10}".format(op, size, cv, pv, ratio))

    print("\n=== Memory (kB; vmrss/vmhwm by phase) ===")
    print("{:<22}{:>14}{:>14}".format("metric", "c", "cpp"))
    for metric, cv, pv in _memory_rows(results):
        print("{:<22}{:>14}{:>14}".format(metric, cv, pv))

    print("\n=== Static footprint ===")
    for api in APIS:
        r = results[api]
        peak = r["peak_rss_kb"]
        print("  {:<4} mex={} bytes  libs={}  whole-proc-peak={}".format(
            api, r["mex_bytes"], ",".join(r["libs"]) or "(none)",
            "{} kB".format(peak) if peak else "n/a"))


def _write_csv(path, results, sizes):
    with open(path, "w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(["op", "size", "c_us", "cpp_us", "c_over_cpp"])
        for row in _timing_table(results, sizes):
            w.writerow(row)
        w.writerow([])
        w.writerow(["mem_metric", "c_kb", "cpp_kb"])
        for row in _memory_rows(results):
            w.writerow(row)
        w.writerow([])
        w.writerow(["api", "mex_bytes", "libs", "whole_proc_peak_kb"])
        for api in APIS:
            r = results[api]
            w.writerow([api, r["mex_bytes"], ";".join(
                r["libs"]), r["peak_rss_kb"]])


def _write_md(path, results, sizes):
    lines = ["# C vs C++ MEX benchmark results", "",
             "Generated by `tests/matlab_runtime/run_benchmark.py`.", "",
             "## Timing (median microseconds per call)", "",
             "| op | size | c (us) | cpp (us) | c/cpp |",
             "| --- | ---: | ---: | ---: | ---: |"]
    for op, size, cv, pv, ratio in _timing_table(results, sizes):
        lines.append("| {} | {} | {} | {} | {} |".format(
            op, size, cv, pv, ratio))
    lines += ["", "## Memory (kB)", "", "| metric | c | cpp |",
              "| --- | ---: | ---: |"]
    for metric, cv, pv in _memory_rows(results):
        lines.append("| {} | {} | {} |".format(metric, cv, pv))
    lines += ["", "## Static footprint", "",
              "| api | mex bytes | linked libs | whole-proc peak (kB) |",
              "| --- | ---: | --- | ---: |"]
    for api in APIS:
        r = results[api]
        lines.append("| {} | {} | {} | {} |".format(
            api, r["mex_bytes"], ", ".join(r["libs"]) or "(none)",
            r["peak_rss_kb"] if r["peak_rss_kb"] else "n/a"))
    with open(path, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--iters", type=int, default=2000,
                        help="reps per size-independent op (default 2000)")
    parser.add_argument("--sizes", default="8,64,256,1024",
                        help="comma-separated matrix/vector sizes")
    parser.add_argument("--out-dir", default=None,
                        help="write results.csv and results.md here")
    parser.add_argument("--time-v", action="store_true",
                        help="cross-check whole-process peak RSS via /usr/bin/time -v")
    args = parser.parse_args(argv)

    if not (_harness.HAVE_MEX and _harness.HAVE_GTSAM):
        parser.error("requires MATLAB (mex) and gtsam headers/libs")

    sizes = [int(s) for s in args.sizes.split(",") if s.strip()]
    results = {}
    for api in APIS:
        print("building + running backend: {} ...".format(api))
        results[api] = run_one(api, args.iters, sizes, args.time_v)

    _print_report(results, sizes)

    if args.out_dir:
        os.makedirs(args.out_dir, exist_ok=True)
        _write_csv(osp.join(args.out_dir, "results.csv"), results, sizes)
        _write_md(osp.join(args.out_dir, "results.md"), results, sizes)
        print("\nwrote {}/results.csv and results.md".format(args.out_dir))
    return 0


if __name__ == "__main__":
    sys.exit(main())
