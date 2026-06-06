# Benchmark: C MEX API vs C++ MEX API backends

This is the empirical companion to `cpp-mex-api-expansion.md` (the design of the
C++ MEX API target) and `cxx-mex-api-review-request.md` (its review). Those
documents establish that the two backends are at functional parity and selectable
at codegen time (`scripts/matlab_wrap.py --mex-api {c,cpp}`, or the
`WRAP_MEX_API` CMake cache option). They had never been compared on performance
or memory. This document measures both and gives a recommendation, tying each
observed number back to the documented architectural difference that causes it.

The generated `.m` classdef is byte-identical between the two targets, so the
MATLAB-side dispatch cost is identical across backends. Every difference measured
here is attributable to the MEX runtime: `matlab.h` (legacy C, `mxArray*`) vs
`matlab_cpp.h` (modern C++, `matlab::data::Array` + a per-call
`Context{engine, factory}`).

## Methodology

Fixture: `tests/matlab_runtime/runtime_demo.h` / `.i` (the same `demo::Counter`
used by the runtime conformance gate), extended with a few trivial echo methods
whose bodies are near-empty so timing isolates wrap/unwrap marshalling rather
than computation: `echoVector`, `echoMatrix`, `traceMatrixView`, `noop`,
`echoInt`, `echoBool`.

Driver: `tests/matlab_runtime/run_benchmark.m`. For each operation it runs a
block-median timing scheme (median over 15 tight loops; reported as microseconds
per call) after an untimed warmup that loads the module and primes the allocator.
Operations:

| op | what it measures | cause under test |
| --- | --- | --- |
| `noop` | method dispatch + handle decode, no value marshalling | fixed per-call overhead |
| `scalar_int` / `scalar_bool` | scalar round-trip | scalar wrap/unwrap; bool storage |
| `vector_echo` | `gtsam::Vector` arg unwrap + return wrap | vector marshalling |
| `matrix_echo` | `gtsam::Matrix` unwrap + wrap (headline) | nested-loop vs bulk `std::copy` |
| `matrix_unwrap` | `traceMatrix` (arg unwrap, scalar return) | unwrap path only |
| `matrix_view_unwrap` | `traceMatrixView` (`ConstMatrixView` arg, scalar return) | zero-copy matrix-view unwrap path |
| `matrix_wrap` | `ramp(n,n)` (return wrap; includes an O(n*n) fill) | wrap path only |
| `construct_destroy` | `Counter(5)` + `delete` | object lifetime (mexLock vs RAII) |
| `error_path` | `mayThrow(true)` try/catch | `feval("error")` vs native `MATLABException` |

Memory, two dimensions:

- Dynamic, in-process (Linux): `run_benchmark.m` reads `/proc/<pid>/status`
  (`feature('getpid')`) for `VmRSS` (current) and `VmHWM` (peak) at phase
  boundaries (baseline after warmup, after the matrix sweep, after a
  construct/destroy pool, at end after `clear mex`).
- Static: the compiled `.mex*` file size and the libraries it links (`ldd`).
  Cross-checked with a whole-process peak RSS from `/usr/bin/time -v`.

The benchmark compiles through `mex` directly (the same flags as the runtime
gate: `-std=c++17`, plus `-DMX_COMPAT_32` for the C target), not through
`cmake/MatlabWrap.cmake`. Numbers therefore reflect the gate's compile
invocation, which is the established passing baseline; a from-CMake build differs
only in flags (`-DMATLAB_DEFAULT_RELEASE=R2021b` and explicit data-array
linkage) and is not expected to change the qualitative picture.

## How to reproduce

```sh
# Smoke gate (small N; asserts only that both backends complete and emit
# structurally sane BENCH output). Auto-skips without MATLAB/mex/gtsam.
pytest tests/test_matlab_runtime_benchmark.py

# Full benchmark with real numbers + comparison tables (CSV + markdown).
python tests/matlab_runtime/run_benchmark.py \
    --iters 2000 --sizes 8,64,256,1024 --time-v \
    --out-dir /tmp/gtwrap-const-matrix-view-benchmark
```

The runner builds and runs both backends, scrapes the `BENCH,...` lines, records
`.mex` size and linked libs, and writes `results.csv` / `results.md` to
`--out-dir`. If MATLAB fails to load the C++ `.mex` with a `GLIBCXX_*` error, the
harness already preloads the toolchain's `libstdc++` (MathWorks' documented
remedy); see `tests/matlab_runtime/_harness.py`.

## Results

Host: Intel Core i7-14700K, MATLAB R2023b Update 10, libgtsam 4.3a1, 2000
iterations, sizes 8/64/256/1024. Single machine, single run; treat small deltas
as noise.

Raw artifacts from the matrix-view benchmark run:

- `/tmp/gtwrap-const-matrix-view-benchmark/results.csv`
- `/tmp/gtwrap-const-matrix-view-benchmark/results.md`

### Timing (median microseconds per call)

Ratio is C / C++: a value below 1.00 means the C backend is faster.

| op | size | c (us) | cpp (us) | c/cpp |
| --- | ---: | ---: | ---: | ---: |
| construct_destroy | 0 | 11.3920 | 19.9800 | 0.57x |
| error_path | 0 | 132.1960 | 188.2900 | 0.70x |
| matrix_echo | 8 | 4.9425 | 38.0355 | 0.13x |
| matrix_echo | 64 | 9.7720 | 27.6720 | 0.35x |
| matrix_echo | 256 | 70.5238 | 76.8413 | 0.92x |
| matrix_echo | 1024 | 2979.5000 | 2697.4000 | 1.10x |
| matrix_unwrap | 8 | 5.1465 | 38.9050 | 0.13x |
| matrix_unwrap | 64 | 7.1280 | 26.9680 | 0.26x |
| matrix_unwrap | 256 | 31.8889 | 41.5714 | 0.77x |
| matrix_unwrap | 1024 | 430.2500 | 376.9500 | 1.14x |
| matrix_view_unwrap | 8 | 5.0430 | 36.0585 | 0.14x |
| matrix_view_unwrap | 64 | 6.8440 | 29.2880 | 0.23x |
| matrix_view_unwrap | 256 | 25.4127 | 39.7460 | 0.64x |
| matrix_view_unwrap | 1024 | 385.1000 | 381.7500 | 1.01x |
| matrix_wrap | 8 | 5.2975 | 40.1055 | 0.13x |
| matrix_wrap | 64 | 10.5960 | 29.3760 | 0.36x |
| matrix_wrap | 256 | 175.2857 | 173.0794 | 1.01x |
| matrix_wrap | 1024 | 4495.3500 | 3971.3000 | 1.13x |
| noop | 0 | 3.3865 | 31.8135 | 0.11x |
| scalar_bool | 0 | 5.0075 | 36.9005 | 0.14x |
| scalar_int | 0 | 5.1795 | 38.2315 | 0.14x |
| vector_echo | 8 | 4.9180 | 35.8610 | 0.14x |
| vector_echo | 64 | 3.3080 | 26.2240 | 0.13x |
| vector_echo | 256 | 5.4444 | 25.6825 | 0.21x |
| vector_echo | 1024 | 6.0000 | 23.3000 | 0.26x |

### Memory (kB)

| metric | c | cpp |
| --- | ---: | ---: |
| vmhwm_baseline | 1468944 | 1469984 |
| vmhwm_end | 1511116 | 3047648 |
| vmhwm_lifecycle | 1511116 | 3047648 |
| vmhwm_matrix | 1511116 | 3047648 |
| vmrss_baseline | 1455760 | 1457280 |
| vmrss_end | 1478604 | 3032012 |
| vmrss_lifecycle | 1478604 | 3032324 |
| vmrss_matrix | 1478604 | 3032100 |

### Static footprint

| api | mex bytes | linked libs | whole-proc peak (kB) |
| --- | ---: | --- | ---: |
| c | 101600 | libmex, libmx | 1511116 |
| cpp | 632480 | libMatlabDataArray, libmex | 3047648 |

## Interpretation

1. The C++ backend still carries a large fixed method-call overhead. The
   matrix-view benchmark `noop` path is 31.81 us for C++ vs 3.39 us for C. That
   fixed cost is still dominated by the C++ MEX API's
   `matlab::engine::MATLABEngine` / Data Array machinery and by handle decode
   through `engine->getProperty`.

2. The zero-copy `WrapIn` view removes avoidable wrapper-side input copying, but
   it does not make the C++ backend faster than C on small or medium calls. That
   is expected and is not a release criterion for the C++ backend.

3. `ConstMatrixView` is now benchmarked directly as `matrix_view_unwrap`.
   Against the copy-unwrapping `matrix_unwrap` path, the C backend improves from
   31.89 us to 25.41 us at size 256 and from 430.25 us to 385.10 us at size
   1024. The C++ backend is mixed in this run: 41.57 us to 39.75 us at size 256,
   but 376.95 us to 381.75 us at size 1024, effectively parity within run noise.
   The feature is therefore a clear C-runtime copy avoidance and a correctness/
   API-extension path for C++, not a guaranteed C++ speed win.

4. Large matrix return wrapping still crosses over in this run: `matrix_wrap`
   at 1024 is 1.13x faster on the C++ backend. Full matrix echo and matrix
   unwrap remain workload-dependent, so dense-matrix throughput should not be
   treated as a blanket C++ advantage.

5. `vector_echo` never crosses over in this range. Even at 1024 elements, the
   vector payload is too small for bulk copy to overcome the C++ method-call
   overhead.

6. The error path is expensive on both backends (exception unwinding through the
   MEX boundary), and is ~1.4x more expensive on the C++ path despite throwing a
   native `MATLABException` rather than the C path's `feval("error", ...)` string
   round-trip. The native exception buys identifier fidelity (the `wrap:error`
   identifier survives the round trip, asserted in `run_test.m`), not speed.

7. Memory is the C++ backend's clearest cost. Peak RSS at end is ~1.51 GB (C) vs
   ~3.05 GB (C++), about 1.5 GB more, driven by the MATLAB Data Array / in-process
   engine machinery the C++ runtime pulls in (`libMatlabDataArray`). Baselines are
   nearly equal (~1.46 GB), so the gap is incurred by *using* the C++ runtime, not
   merely loading it. The compiled artifact is also ~6x larger (632 KB vs 102 KB).
   The documented `bool`-as-`logical` (1 byte) vs `bool`-as-`uint32` (8 bytes)
   difference favours C++ but is negligible at this scale and invisible against
   the engine footprint.

## Recommendation

Prefer the C MEX API (the default, `WRAP_MEX_API=c`) for performance- or
memory-sensitive bindings. It remains much faster on the small/medium calls that
dominate typical interactive MATLAB use, and uses roughly half the peak memory.

Choose the C++ MEX API (`WRAP_MEX_API=cpp`) for its correctness and maintenance
properties, not for speed:

- RAII lifetime via the `MexFunction` destructor instead of
  `mexLock`/`mexAtExit` (no manual lock bookkeeping; cleaner clear/reload/exit).
- Native `MATLABException` with a preserved error identifier.
- `bool` returned as MATLAB `logical`, matching MATLAB semantics.
- A modern, type-checked API surface that is easier to extend (see
  `extended-type-support.md`).

Do not require the C++ backend to be faster than the C backend to consider the
implementation complete. The C++ backend closes on correctness, teardown safety,
and maintainability; the benchmark simply documents the cost profile and keeps
the default recommendation honest.

## Threats to validity

- Single machine, single run; small ratio differences (within ~10-20%) are
  jitter, not signal. Re-run with larger `--iters` for tighter medians.
- Process RSS includes MATLAB's multi-hundred-MB baseline; the meaningful figure
  is the C-vs-C++ delta and `VmHWM` peak, reported above, not absolute RSS.
- `matrix_wrap` uses `ramp`, which fills the matrix (O(n*n)) in addition to
  wrapping it, so it slightly understates the pure wrap-copy gap.
- Numbers reflect the direct-`mex` compile (the runtime gate's flags), not a
  from-`cmake/MatlabWrap.cmake` build; the qualitative picture is not expected to
  change.
