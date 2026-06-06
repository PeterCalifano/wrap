"""Smoke gate for the C-vs-C++ MEX benchmark.

For each MEX API target ('c' and 'cpp') this generates and compiles the
runtime_demo wrapper and runs run_benchmark.m with a small iteration count. It
asserts only that the benchmark COMPLETES and emits structurally sane output
(the completion sentinel plus at least one BENCH line of each kind). It does NOT
assert relative speed or absolute memory - those are environment-dependent and
are reported empirically by tests/matlab_runtime/run_benchmark.py, not gated
here.

Skipped unless MATLAB (mex) and gtsam headers/libs are available.
"""
# pylint: disable=import-error, wrong-import-position

import os.path as osp
import re
import shutil
import sys
import tempfile
import unittest

sys.path.append(osp.join(osp.dirname(osp.abspath(__file__)), "matlab_runtime"))

import _harness  # noqa: E402

BENCH_RE = re.compile(r"^BENCH,([^,]+),([^,]+),([^,]+),([^,]+),(.+)$", re.M)


@unittest.skipUnless(
    _harness.HAVE_MEX and _harness.HAVE_GTSAM,
    "requires MATLAB (mex) and gtsam headers/libs")
class TestMatlabRuntimeBenchmark(unittest.TestCase):
    """Build and run the benchmark driver for both MEX APIs (smoke only)."""

    def test_benchmark_runs(self):
        for api in ("c", "cpp"):
            with self.subTest(api=api):
                out_dir = tempfile.mkdtemp(prefix="gtwrap_bench_{}_".format(api))
                try:
                    _harness.generate(out_dir, api)
                    _harness.compile_wrapper(out_dir, api)
                    call = "run_benchmark('{}','{}',50,[8 32])".format(out_dir, api)
                    result = _harness.run_matlab(
                        call, expect="ALL BENCHMARKS COMPLETE", timeout=600)
                    self.assertEqual(result.returncode, 0,
                                     "benchmark ({}) exited non-zero:\n{}".format(
                                         api, result.output))
                    rows = BENCH_RE.findall(result.output)
                    self.assertTrue(rows, "no BENCH lines emitted for api={}".format(api))
                    ops = {r[0] for r in rows}
                    # A timing row, a vmrss snapshot and a vmhwm snapshot.
                    self.assertIn("matrix_echo", ops)
                    self.assertTrue(any(o.startswith("vmrss_") for o in ops),
                                    "no vmrss memory rows for api={}".format(api))
                    self.assertTrue(any(o.startswith("vmhwm_") for o in ops),
                                    "no vmhwm memory rows for api={}".format(api))
                finally:
                    shutil.rmtree(out_dir, ignore_errors=True)


if __name__ == "__main__":
    unittest.main()
