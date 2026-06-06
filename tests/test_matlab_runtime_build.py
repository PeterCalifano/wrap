"""End-to-end build+run tests for the generated MATLAB wrappers.

For each MEX API target ('c' and 'cpp') this generates the runtime_demo
wrapper, compiles it with the MATLAB `mex` compiler against real libgtsam, and
runs three MATLAB drivers:

  run_test     - functional conformance (scalars, strings, Eigen, enums,
                 virtual dispatch, raw-pointer args, errors, lifetime);
  run_cleanup reload - use the wrapper, `clear all`, then create new objects;
  run_cleanup exit   - leave live objects and let MATLAB unload on exit.

The cleanup drivers guard against the clear/reload/exit segfaults the C MEX
path has historically suffered. The whole module is skipped when MATLAB, the
mex compiler, or gtsam headers/libs are not available.

The generate/compile/invoke primitives live in matlab_runtime/_harness.py and
are shared with the C-vs-C++ benchmark (test_matlab_runtime_benchmark.py).
"""
# pylint: disable=import-error, wrong-import-position

import glob
import os.path as osp
import shutil
import subprocess
import sys
import tempfile
import textwrap
import unittest

sys.path.append(osp.join(osp.dirname(osp.abspath(__file__)), "matlab_runtime"))

import _harness  # noqa: E402

# Re-exported from the shared harness so the CMake consumer below can build its
# generated CMakeLists against the same paths the mex path uses.
REPO_ROOT = _harness.REPO_ROOT
WRAP_INCLUDE_PARENT = _harness.WRAP_INCLUDE_PARENT
RUNTIME_DIR = _harness.RUNTIME_DIR
WRAP_SCRIPT = _harness.WRAP_SCRIPT
GTSAM_INCLUDE = _harness.GTSAM_INCLUDE
GTSAM_LIB = _harness.GTSAM_LIB
EIGEN_INCLUDE = _harness.EIGEN_INCLUDE

HAVE_CMAKE = shutil.which("cmake") is not None


@unittest.skipUnless(
    _harness.HAVE_MEX and _harness.HAVE_GTSAM,
    "requires MATLAB (mex) and gtsam headers/libs")
class TestMatlabRuntimeBuild(unittest.TestCase):
    """Compile and run the generated wrappers in MATLAB, for both MEX APIs."""

    def test_build_and_run(self):
        for api in ("c", "cpp"):
            with self.subTest(api=api):
                out_dir = tempfile.mkdtemp(prefix=f"gtwrap_{api}_")
                try:
                    self._generate(out_dir, api)
                    self._compile(out_dir, api)
                    # Functional conformance.
                    self._matlab(f"run_test('{out_dir}','{api}')")
                    # Teardown safety: clear-and-reload, and live-at-exit.
                    self._matlab(f"run_cleanup('{out_dir}','reload')",
                                 expect="CLEANUP reload OK")
                    self._matlab(f"run_cleanup('{out_dir}','exit')",
                                 expect="CLEANUP exit OK")
                finally:
                    shutil.rmtree(out_dir, ignore_errors=True)

    @unittest.skipUnless(HAVE_CMAKE, "requires cmake")
    def test_cmake_cpp_api_build_and_run(self):
        """Build the C++ MEX API wrapper through MatlabWrap.cmake."""
        src_dir = tempfile.mkdtemp(prefix="gtwrap_cmake_src_")
        build_dir = tempfile.mkdtemp(prefix="gtwrap_cmake_build_")
        try:
            self._write_cmake_consumer(src_dir)
            self._cmake_configure(src_dir, build_dir)
            self._cmake_build(build_dir)

            generated_dir = osp.join(build_dir, "wrap", "runtime_demo")
            mex_dir = osp.join(build_dir, "wrap", "runtime_demo_mex")
            mex_files = glob.glob(osp.join(mex_dir, "runtime_demo_wrapper.mex*"))
            self.assertTrue(mex_files, "CMake build produced no runtime_demo mex")
            shutil.copy2(mex_files[0], generated_dir)

            self._matlab(f"run_test('{generated_dir}','cpp')")
        finally:
            shutil.rmtree(src_dir, ignore_errors=True)
            shutil.rmtree(build_dir, ignore_errors=True)

    def _generate(self, out_dir, api):
        try:
            _harness.generate(out_dir, api)
        except RuntimeError as exc:
            self.fail(str(exc))
        self.assertTrue(
            osp.exists(osp.join(out_dir, "runtime_demo_wrapper.cpp")))

    def _write_cmake_consumer(self, src_dir):
        cmake_lists = textwrap.dedent(f"""\
            cmake_minimum_required(VERSION 3.22)
            project(gtwrap_runtime_demo LANGUAGES CXX)

            set(GTWRAP_PACKAGE_DIR "{REPO_ROOT}")
            set(MATLAB_WRAP_SCRIPT "{WRAP_SCRIPT}" CACHE PATH "")
            set(PYTHON_EXECUTABLE "{sys.executable}" CACHE PATH "")
            include("{osp.join(REPO_ROOT, 'cmake', 'MatlabWrap.cmake' )}")

            add_library(runtime_demo INTERFACE)
            target_include_directories(runtime_demo INTERFACE
              "{RUNTIME_DIR}"
              "{WRAP_INCLUDE_PARENT}"
              "{REPO_ROOT}"
              "{GTSAM_INCLUDE}"
              "{EIGEN_INCLUDE}")
            target_link_directories(runtime_demo INTERFACE "{GTSAM_LIB}")
            target_link_libraries(runtime_demo INTERFACE gtsam)

            find_and_configure_matlab()
            wrap_library_internal(
              "{osp.join(RUNTIME_DIR, 'runtime_demo.i')}"
              runtime_demo
              ""
              "{RUNTIME_DIR};{REPO_ROOT};{WRAP_INCLUDE_PARENT};{GTSAM_INCLUDE};{EIGEN_INCLUDE}"
              ""
              ""
              OFF)
            """)
        with open(osp.join(src_dir, "CMakeLists.txt"), "w",
                  encoding="UTF-8") as f:
            f.write(cmake_lists)

    def _cmake_configure(self, src_dir, build_dir):
        proc = subprocess.run(
            ["cmake", "-S", src_dir, "-B", build_dir, "-DWRAP_MEX_API=cpp"],
            capture_output=True, text=True)
        if proc.returncode != 0:
            self.fail("CMake configure failed:\n" + proc.stdout + proc.stderr)

    def _cmake_build(self, build_dir):
        proc = subprocess.run(
            ["cmake", "--build", build_dir, "--target",
             "runtime_demo_matlab_wrapper", "--parallel", "2"],
            capture_output=True, text=True)
        if proc.returncode != 0:
            self.fail("CMake build failed:\n" + proc.stdout + proc.stderr)

    def _compile(self, out_dir, api):
        try:
            _harness.compile_wrapper(out_dir, api)
        except RuntimeError as exc:
            self.fail(str(exc))
        self.assertTrue(
            glob.glob(osp.join(out_dir, "runtime_demo_wrapper.mex*")),
            f"no compiled mex produced for api={api}")

    def _matlab(self, call, expect="ALL RUNTIME CHECKS PASSED"):
        result = _harness.run_matlab(call, timeout=600)
        self.assertIn(expect, result.output,
                      f"MATLAB `{call}` failed:\n" + result.output)
        self.assertEqual(result.returncode, 0,
                         f"MATLAB `{call}` exited non-zero:\n" + result.output)


if __name__ == "__main__":
    unittest.main()
