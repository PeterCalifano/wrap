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
than computation: `echoVector`, `echoMatrix`, `noop`, `echoInt`, `echoBool`.

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
    --out-dir /tmp/gtwrap-c-vs-cpp-mex-after-fixes
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

Raw artifacts from the post-fix run:

- `/tmp/gtwrap-c-vs-cpp-mex-after-fixes/results.csv`
- `/tmp/gtwrap-c-vs-cpp-mex-after-fixes/results.md`

### Timing (median microseconds per call)

Ratio is C / C++: a value below 1.00 means the C backend is faster.

| op | size | c (us) | cpp (us) | c/cpp |
| --- | ---: | ---: | ---: | ---: |
| construct_destroy | 0 | 6.9020 | 13.3380 | 0.52x |
| error_path | 0 | 83.1540 | 134.9580 | 0.62x |
| matrix_echo | 8 | 3.0235 | 26.5280 | 0.11x |
| matrix_echo | 64 | 5.9480 | 27.2600 | 0.22x |
| matrix_echo | 256 | 51.9206 | 73.3651 | 0.71x |
| matrix_echo | 1024 | 2611.4500 | 5278.8000 | 0.49x |
| matrix_unwrap | 8 | 2.9750 | 25.9790 | 0.11x |
| matrix_unwrap | 64 | 4.2800 | 25.9000 | 0.17x |
| matrix_unwrap | 256 | 18.1746 | 39.7937 | 0.46x |
| matrix_unwrap | 1024 | 364.4500 | 799.1500 | 0.46x |
| matrix_wrap | 8 | 3.4090 | 28.5080 | 0.12x |
| matrix_wrap | 64 | 6.6560 | 33.0400 | 0.20x |
| matrix_wrap | 256 | 147.0317 | 166.3492 | 0.88x |
| matrix_wrap | 1024 | 4584.8000 | 4466.5500 | 1.03x |
| noop | 0 | 1.9610 | 22.1695 | 0.09x |
| scalar_bool | 0 | 3.0845 | 25.2110 | 0.12x |
| scalar_int | 0 | 8.3320 | 26.0120 | 0.32x |
| vector_echo | 8 | 3.0015 | 25.5170 | 0.12x |
| vector_echo | 64 | 3.2400 | 28.8280 | 0.11x |
| vector_echo | 256 | 3.5079 | 30.4286 | 0.12x |
| vector_echo | 1024 | 3.6500 | 23.2000 | 0.16x |

### Memory (kB)

| metric | c | cpp |
| --- | ---: | ---: |
| vmhwm_baseline | 1486852 | 1476548 |
| vmhwm_end | 1522540 | 2819244 |
| vmhwm_lifecycle | 1522540 | 2819244 |
| vmhwm_matrix | 1522540 | 2819244 |
| vmrss_baseline | 1471832 | 1461980 |
| vmrss_end | 1490000 | 2810892 |
| vmrss_lifecycle | 1490000 | 2811176 |
| vmrss_matrix | 1490000 | 2810952 |

### Static footprint

| api | mex bytes | linked libs | whole-proc peak (kB) |
| --- | ---: | --- | ---: |
| c | 101232 | libmex, libmx | 1522540 |
| cpp | 627976 | libMatlabDataArray, libmex | 2819244 |

## Interpretation

1. The C++ backend still carries a large fixed method-call overhead. The
   post-fix `noop` path is 22.17 us for C++ vs 1.96 us for C. This is better
   than the earlier 31.75 us C++ `noop` measurement after removing the copied
   input vector and per-call RTTI workspace check, but it is still dominated by
   the C++ MEX API's `matlab::engine::MATLABEngine` / Data Array machinery and
   by handle decode through `engine->getProperty`.

2. The zero-copy `WrapIn` view removes avoidable wrapper-side input copying, but
   it does not make the C++ backend faster than C on small or medium calls. That
   is expected and is not a release criterion for the C++ backend.

3. Large matrix return wrapping is the only case that reaches parity in this
   run: `matrix_wrap` at 1024 is effectively tied, with C++ 1.03x faster. Full
   matrix echo and matrix unwrap remain C-favoured in this run, so dense-matrix
   throughput should be treated as workload-dependent rather than a blanket C++
   advantage.

4. `vector_echo` never crosses over in this range: vectors up to 1024 elements
   are too small for bulk copy to overcome the C++ method-call overhead.

5. The error path is expensive on both backends (exception unwinding through the
   MEX boundary), and is ~1.6x more expensive on the C++ path despite throwing a
   native `MATLABException` rather than the C path's `feval("error", ...)` string
   round-trip. The native exception buys identifier fidelity (the `wrap:error`
   identifier survives the round trip, asserted in `run_test.m`), not speed.

6. Memory is the C++ backend's clearest cost. Peak RSS at end is ~1.52 GB (C) vs
   ~2.82 GB (C++), about 1.3 GB more, driven by the MATLAB Data Array / in-process
   engine machinery the C++ runtime pulls in (`libMatlabDataArray`). Baselines are
   nearly equal (~1.47 GB), so the gap is incurred by *using* the C++ runtime, not
   merely loading it. The compiled artifact is also ~6x larger (628 KB vs 101 KB).
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
