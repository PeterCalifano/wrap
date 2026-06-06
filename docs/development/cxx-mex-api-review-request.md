# Review request: C++ MEX API MATLAB target

Audience: Codex (bug/gap hunt). This document recaps the implementation status
of the modern C++ MEX API MATLAB target, summarizes the key changes, and lists
the specific areas to scrutinize.

## 1. What this change does

The MATLAB wrapper historically emitted only the legacy C MEX API (`mex.h`:
`mexFunction`, `mxArray*`, `mexLock`, `mexErrMsgTxt`). This change adds a second,
selectable backend that emits against the modern C++ MEX API (`mex.hpp` /
`mexAdapter.hpp`, the MATLAB Data Array API: `matlab::mex::Function`,
`matlab::data::Array`, `ArrayFactory`, `matlab::engine::MATLABEngine`).

The two backends are at functional parity. The selection is made at codegen time:

- `scripts/matlab_wrap.py --mex-api {c,cpp}` (default `c`).
- CMake cache option `WRAP_MEX_API` (`c` | `cpp`, default `c`) in
  `cmake/MatlabWrap.cmake`.

Runtime headers included by the generated `.cpp`: `matlab.h` (C API, legacy) and
`matlab_cpp.h` (C++ API, new). Both are meant to be at feature parity.

## 2. Design contract (the invariants a reviewer should verify hold)

1. `MatlabWrapperCpp` is a SUBCLASS of `MatlabWrapper`, not a fork. All C++
   divergence is isolated in a small set of overridable hooks. The base AST walk
   does not branch on the target; it calls hooks that the C++ subclass overrides.
2. The C path output is FROZEN. Any change to `wrapper.py` must leave
   `tests/expected/matlab/*.cpp` byte-identical. The golden files are the gate.
3. The generated `.m` classdef files are byte-identical between the two targets.
   Only the generated `_wrapper.cpp` and the runtime header differ. Both targets
   expose the same `module_wrapper(id, this, args...)` callable and the
   pointer-key constructor protocol (`ptr_constructor_key`).
4. Pybind/Python target must not regress.

## 3. Key files changed

Generator:

- `gtwrap/matlab_wrapper/wrapper.py` (base, C path): added neutral hooks with
  C-path defaults so the base walk is target-agnostic: `_ctx_arg()` (returns
  `''`), `_runtime_include()`, `_collector_signature`, `_collector_*_body`,
  `_mex_function_cases`, and class-level template attributes
  (`_delete_all_objects_template`, `_rtti_register_template`,
  `_upcast_from_void_template`). Added `{ctx}` placeholders at the shared emit
  sites for unwrap/wrap of shared pointers and enums (empty string on the C
  path, so output is unchanged).
- `gtwrap/matlab_wrapper/wrapper_cpp.py` (new, ~109 lines): `MatlabWrapperCpp`
  overrides `_runtime_include`, `_ctx_arg` (returns `'ctx, '`),
  `_include_namespace`, `_collector_signature` (prepends `Context& ctx,`),
  `_collector_insert_make_base_body`, `_collector_constructor_body`,
  `_collector_deconstructor_body`, and `mex_function`.
- `gtwrap/matlab_wrapper/templates.py` (C templates): added `{ctx}` to the
  serialize/deserialize/shared-return fragments (byte-identical for C).
- `gtwrap/matlab_wrapper/templates_cpp.py` (new, ~121 lines): `WrapperTemplateCpp`
  with the C++ forms of `delete_all_objects`, `rtti_register`,
  `collector_function_upcast_from_void`, and `mex_function` (the `MexFunction`
  class).

Runtime headers:

- `matlab_cpp.h` (new, ~619 lines): the C++ MEX runtime. `struct Context {
  matlab::engine::MATLABEngine* engine; matlab::data::ArrayFactory factory; }`,
  threaded per call. `wrap<T>` / `unwrap<T>` scalar specializations,
  `wrap_Vector` / `wrap_Matrix` (double Eigen), `make_handle` / `get_handle`
  (uint64 handle, factory-only, no engine), `wrap_enum` / `unwrap_enum`,
  `create_object`, `wrap_shared_ptr` / `unwrap_shared_ptr` / `unwrap_ptr`.
  `[[noreturn]] MexErrMsgIdAndTxt` throws `matlab::engine::MATLABException`.
- `matlab.h` (C runtime): the only functional change is the `unwrap_ptr`
  bugfix (see section 5).

Build / entry points:

- `cmake/MatlabWrap.cmake`: `WRAP_MEX_API` cache option; cpp path adds
  `-DMATLAB_DEFAULT_RELEASE=R2021b`, enforces C++17, links the MATLAB Data Array
  libs (`libMatlabDataArray`, plus `libmex` on Linux for the in-process engine).
- `CMakeLists.txt`: installs both `matlab.h` and `matlab_cpp.h`.
- `scripts/matlab_wrap.py`: `--mex-api` argument; factory selects the wrapper.

Tests:

- `tests/expected/matlab_cpp/*.cpp`: 9 new golden fixtures (C++ target).
- `tests/test_matlab_wrapper.py`: `TestWrapCpp` (golden match, safe-handle
  assertions, scalar contracts, entry-point structure).
- `tests/matlab_runtime/` and `tests/test_matlab_runtime_build.py`: MATLAB-gated
  end-to-end build+run gate, parametrized over `('c','cpp')`, against real
  `libgtsam`. Auto-skips when MATLAB/mex/gtsam absent.

## 4. Test status

`pytest tests/` -> 103 passed. Codegen goldens for both C and C++ targets,
pybind unaffected, both-API MATLAB build+cleanup gate (when MATLAB present).
C path `.cpp` and the shared `.m` files are byte-identical. Pybind untouched.

## 5. Bugs found and fixed during this work (context for the reviewer)

- C `unwrap_ptr` returned the address of the storage cell
  (`reinterpret_cast<Class*>(mxGetData(...))`) instead of dereferencing the
  stored `shared_ptr<Class>*` and calling `.get()`. Fixed in `matlab.h`; report
  in `docs/development/bug-report-c-unwrap-ptr.md`.
- C++ `getEngine()` result was used transiently and could dangle in
  `mexApiSetVariable`. Fixed by holding a `std::shared_ptr<MATLABEngine>` local
  in `operator()` and the destructor.
- On R2023b, `matlab::mex::ArgumentList` is an incomplete type unless
  `mexAdapter.hpp` is included before the `MexFunction` class. The include now
  sits at the top of `matlab_cpp.h`.

## 6. Where to look hardest (highest-risk areas)

1. Parity drift between `matlab.h` and `matlab_cpp.h`: any `wrap`/`unwrap`
   specialization present in one but missing/divergent in the other; differing
   error messages or validation strictness; differing handling of empty arrays,
   row vs column vectors, and scalar-vs-1x1 cases.
2. The `Context` lifetime in `matlab_cpp.h` / the `MexFunction` class: confirm
   the engine `shared_ptr` cannot dangle across `feval`/`getProperty`/
   `setVariable`, in both the success path and the exception path, and in the
   destructor (cleanup at `clear`/MATLAB exit).
3. Hook isolation: confirm the base `wrapper.py` walk never branches on the
   target, and that every C++ divergence is an overridden hook. Any `if cpp:`
   style branch in the base is a contract violation.
4. The `{ctx}` placeholder threading: confirm `_ctx_arg()` is prepended at every
   engine-using helper call and nowhere else (factory-only helpers must NOT get
   a `ctx`), and that the C path renders these as empty (byte-identical golden).
5. Handle safety on the C++ path: the cpp output must use `get_handle` /
   `make_handle` and must NOT use `mexLock` / `mxCreateNumericMatrix`. Check for
   double-free / use-after-free in the constructor/deconstructor collector
   bodies and in `delete_all_objects` / RTTI registry teardown.
6. RTTI registry merge (`rtti_register`): the C++ form merges into a
   `StructArray`. Check for leaks or stale entries across reload, and for the
   "static workspace" pitfall on the MATLAB side.
7. Exception mapping: `MATLABException` is caught and rethrown before the generic
   `std::exception` handler in `mex_function`. Confirm ordering and that error
   IDs/messages survive the round trip; confirm `[[noreturn]]` paths actually do
   not return.
8. ASCII-cleanliness of generated code and runtime headers (no em-dashes,
   unicode arrows, smart quotes); the C++ `->` operator is fine.

## 7. Out of scope for this review

Extended type support (typed `Eigen::Matrix<T,R,C>`, numeric `std::vector<T>`,
`std::vector<string>` -> cellstr, `float` scalar) is PLANNED but NOT implemented;
see `docs/development/extended-type-support.md`. Do not flag those gaps as bugs
in the current target. The current target supports the same type surface as the
legacy C target (double Eigen via `gtsam::Vector`/`Matrix`, the fixed scalar
set, enums, shared/raw pointers, virtual classes). Have a look at the plan.
If any issue found in its design due to assumptions or similar, fix

## 8. How to run

```sh
pytest tests/                                   # full suite (no MATLAB needed)
pytest tests/test_matlab_wrapper.py::TestWrapCpp  # C++ codegen target
# MATLAB-gated end-to-end (auto-skips without matlab/mex/gtsam):
pytest tests/test_matlab_runtime_build.py
```

Background and the full C-vs-C++ API mapping are in
`docs/development/cpp-mex-api-expansion.md`.
