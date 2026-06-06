"""Shared MATLAB build/run primitives for the runtime gate and the benchmark.

These helpers were originally private methods on the runtime build test. They are
factored out here as free functions so both ``test_matlab_runtime_build.py`` (the
correctness gate) and the C-vs-C++ benchmark (``run_benchmark.py`` /
``test_matlab_runtime_benchmark.py``) drive MATLAB through one code path: generate
the wrapper, compile it with ``mex`` against real libgtsam, and invoke a MATLAB
``-batch`` driver. Keeping a single source of truth means the benchmark measures
the very artifact the gate compiles.

The compile flags intentionally mirror the gate (``-std=c++17``; ``-DMX_COMPAT_32``
for the C target) rather than ``cmake/MatlabWrap.cmake``; the gate is the passing
baseline. See ``docs/development/c-vs-cpp-mex-benchmark.md``.
"""
# pylint: disable=import-error

import collections
import glob
import os
import os.path as osp
import re
import shutil
import subprocess
import sys

HARNESS_DIR = osp.dirname(osp.realpath(__file__))   # tests/matlab_runtime
TEST_DIR = osp.dirname(HARNESS_DIR)                  # tests
REPO_ROOT = osp.dirname(TEST_DIR)                    # wrap
WRAP_INCLUDE_PARENT = osp.dirname(REPO_ROOT)        # so `<wrap/matlab*.h>` resolves
RUNTIME_DIR = HARNESS_DIR
WRAP_SCRIPT = osp.join(REPO_ROOT, "scripts", "matlab_wrap.py")

# Locations of the C++ dependencies the demo links against (overridable).
GTSAM_INCLUDE = os.environ.get("GTSAM_INCLUDE_DIR", "/usr/local/include")
GTSAM_LIB = os.environ.get("GTSAM_LIB_DIR", "/usr/local/lib")
EIGEN_INCLUDE = os.environ.get("EIGEN_INCLUDE_DIR", "/usr/include/eigen3")

GNU_TIME = "/usr/bin/time"  # GNU time, for whole-process peak-RSS cross-check


def find_matlab_bindir():
    """Return the MATLAB bin directory containing `matlab` and `mex`, or None."""
    matlab = shutil.which("matlab")
    if matlab:
        return osp.dirname(osp.realpath(matlab))
    for root in sorted(glob.glob("/usr/local/MATLAB/R*") +
                       glob.glob("/opt/MATLAB/R*"), reverse=True):
        if osp.exists(osp.join(root, "bin", "matlab")):
            return osp.join(root, "bin")
    return None


MATLAB_BINDIR = find_matlab_bindir()
HAVE_GTSAM = osp.exists(osp.join(GTSAM_INCLUDE, "gtsam", "base", "Vector.h")) and \
    bool(glob.glob(osp.join(GTSAM_LIB, "libgtsam.so*")))
HAVE_MEX = MATLAB_BINDIR is not None and osp.exists(
    osp.join(MATLAB_BINDIR, "mex"))


def system_libstdcpp():
    """Path to the toolchain's libstdc++ for LD_PRELOAD.

    MATLAB ships an older libstdc++ than a modern system g++; preloading the
    system one is MathWorks' documented remedy for `GLIBCXX_* not found`.
    """
    try:
        out = subprocess.check_output(
            ["g++", "-print-file-name=libstdc++.so.6"], text=True).strip()
        return osp.realpath(out) if osp.isabs(out) else None
    except (OSError, subprocess.CalledProcessError):
        return None


def generate(out_dir, api, module_name="runtime_demo"):
    """Generate ``<module_name>_wrapper.cpp`` for the given MEX api into out_dir.

    Returns the path to the generated wrapper. Raises on failure.
    """
    env = dict(os.environ, PYTHONPATH=REPO_ROOT)
    proc = subprocess.run(
        [sys.executable, WRAP_SCRIPT,
         "--src", osp.join(RUNTIME_DIR, module_name + ".i"),
         "--module_name", module_name,
         "--out", out_dir,
         "--top_module_namespaces", "",
         "--ignore", "",
         "--mex-api", api],
        env=env, capture_output=True, text=True, check=False)
    wrapper = osp.join(out_dir, module_name + "_wrapper.cpp")
    if proc.returncode != 0 or not osp.exists(wrapper):
        raise RuntimeError(
            "codegen ({}) failed:\n{}{}".format(api, proc.stdout, proc.stderr))
    return wrapper


def compile_wrapper(out_dir, api, module_name="runtime_demo"):
    """Compile the generated wrapper with `mex`. Returns the `.mex*` path.

    Mirrors the gate's flags: -std=c++17, plus -DMX_COMPAT_32 for the C target.
    Raises on failure.
    """
    cxxflags = "$CXXFLAGS -std=c++17"
    if api == "c":
        cxxflags += " -DMX_COMPAT_32"  # legacy C MEX API, like the CMake path
    proc = subprocess.run(
        [osp.join(MATLAB_BINDIR, "mex"), "-silent",
         "CXXFLAGS=" + cxxflags,
         "-I" + WRAP_INCLUDE_PARENT,
         "-I" + RUNTIME_DIR,
         "-I" + GTSAM_INCLUDE,
         "-I" + EIGEN_INCLUDE,
         "-L" + GTSAM_LIB, "-lgtsam",
         "-outdir", out_dir,
         osp.join(out_dir, module_name + "_wrapper.cpp")],
        capture_output=True, text=True, check=False)
    if proc.returncode != 0:
        raise RuntimeError(
            "mex compile ({}) failed:\n{}{}".format(api, proc.stdout, proc.stderr))
    mex_files = glob.glob(osp.join(out_dir, module_name + "_wrapper.mex*"))
    if not mex_files:
        raise RuntimeError("no compiled mex produced for api={}".format(api))
    return mex_files[0]


# (combined stdout+stderr, exit code, whole-process peak RSS in kB or None)
RunResult = collections.namedtuple("RunResult", ["output", "returncode", "peak_rss_kb"])

_TIME_V_PEAK = re.compile(r"Maximum resident set size \(kbytes\):\s*(\d+)")


def run_matlab(call, expect=None, time_v=False, timeout=600):
    """Run a MATLAB ``-batch`` call with RUNTIME_DIR on the path.

    When ``time_v`` is set, wrap the invocation in GNU ``/usr/bin/time -v`` and
    parse the whole-process peak RSS (kB). When ``expect`` is given, raise unless
    it appears in the combined output. Returns a ``RunResult``.
    """
    env = dict(os.environ)
    libstdcpp = system_libstdcpp()
    if libstdcpp:
        preload = env.get("LD_PRELOAD", "")
        env["LD_PRELOAD"] = (libstdcpp + ":" + preload) if preload else libstdcpp
    cmd = "addpath('{}'); {}".format(RUNTIME_DIR, call)
    argv = [osp.join(MATLAB_BINDIR, "matlab"), "-batch", cmd]
    use_time_v = time_v and osp.exists(GNU_TIME)
    if use_time_v:
        argv = [GNU_TIME, "-v"] + argv
    proc = subprocess.run(
        argv, env=env, capture_output=True, text=True, timeout=timeout, check=False)
    output = proc.stdout + proc.stderr
    peak = None
    if use_time_v:
        m = _TIME_V_PEAK.search(proc.stderr)
        if m:
            peak = int(m.group(1))
    if expect is not None and expect not in output:
        raise RuntimeError("MATLAB `{}` failed:\n{}".format(call, output))
    return RunResult(output, proc.returncode, peak)
