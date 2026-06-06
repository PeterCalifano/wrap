# Plan: add a C++ MEX API target alongside the existing C API

## Context

This is an expansion, not a migration: the existing C-API generator stays the
default and untouched; a second, selectable target is added that emits the same
wrappers against the modern C++ MEX API.

`gtwrap`'s MATLAB target emits MEX wrappers against the legacy C MEX API
(`mex.h`: `mexFunction`, `mxArray*`, `mxCreate*`, `mxGetData`, `mexLock/Unlock/AtExit`,
`mexCallMATLAB`, `mexGetVariable/PutVariable`). This branch
(`bugfix/fix-matlab-wrap-c-api`) has been fixing crashes rooted in that API
(`cout` redirection, raw-pointer deref). The C API is error-prone: manual
`mxArray` lifetime, `reinterpret_cast` handle encoding, no type safety.

MathWorks' C++ MEX API (`mex.hpp` / `mexAdapter.hpp`, MATLAB Data Array API)
is the supported, memory-safe alternative: a `matlab::mex::Function` subclass
whose object lifetime replaces `mexLock`/`mexAtExit`, value-typed
`matlab::data::Array` with reference counting (no manual destroy), typed
`getProperty`/`setProperty`, and `feval` through a `MATLABEngine` handle.

Goal: add a C++ MEX API target that emits functionally-identical MATLAB
toolboxes, selectable at CMake-configure time, without touching the existing
C-API path. Full parity first; safety improvements the API enables come for free.

Decisions (from user):

- Parallel target, not replacement. New `--mex-api {c,cpp}` flag; default `c`.
- Selected via a CMake option at configure time.
- Min MATLAB release R2021b.
- Generator design: subclass (option B), no in-place branching of the C path.

## Key parity-preserving invariant

Generated `.m` classdef files are unchanged between targets - they call the
single MEX entry point as a function `module_wrapper(id, this, args...)` and use
the pointer-key constructor protocol (`uint64(5139824614673773682)` ==
`ptr_constructor_key`). Both targets expose the same callable to MATLAB, so only
the generated `_wrapper.cpp` and the runtime header differ. New test surface =
the `.cpp` outputs only.

## C API to C++ MEX API mapping (reference for the runtime)

| Legacy C (`mex.h`) | Modern C++ (`mex.hpp`, `matlab::data` / `matlab::engine`) |
| --- | --- |
| `void mexFunction(nargout,out[],nargin,in[])` | `class MexFunction : matlab::mex::Function { void operator()(ArgumentList out, ArgumentList in); }` + `mexAdapter.hpp` |
| `mxArray*` (pointer) | `matlab::data::Array` (value, ref-counted) |
| `mxCreateNumericMatrix`/`mxGetData` reinterpret handle | `ArrayFactory::createScalar<uint64_t>` + `TypedArray<uint64_t>` via `make_handle` / `get_handle` |
| `mxCreateDoubleScalar/Matrix`, `mxGetPr` | `ArrayFactory::createScalar/createArray<double>`, `TypedArray<double>` |
| `mxCreateString` / `mxArrayToString` | `ArrayFactory::createCharArray` / `CharArray::toAscii` |
| `mxGetM/N`, `mxGetClassID`, `mxIsDouble` | `Array::getDimensions()`, `Array::getType()` |
| `mxGetProperty(obj,0,"p")` | `engine->getProperty(obj, u"p")` |
| `mexCallMATLAB` | `engine->feval(u"fn", nout, args)` |
| `mexGetVariable/PutVariable("global",...)` + struct field API | `engine->getVariable/setVariable(name, WorkspaceType::GLOBAL)`; `StructArray` |
| `mexErrMsgTxt/IdAndTxt` | restore cout then `engine->feval(u"error", ...)` |
| `mexPrintf` (cout redirect) | streambuf to `engine->feval(u"fprintf", {1,"%s",text})` |
| `mexLock`/`mexUnlock`/`mexAtExit` | dropped - `MexFunction` lifetime is the lock; `~MexFunction()` runs `_deleteAllObjects()` |

## Architecture (generated C++ wrapper)

Collector-function body shape is preserved via runtime shims, so generator
changes stay localized. Signature becomes
`void Foo_method_N(int nargout, OutputList& out, int nargin, WrapIn in)` - so
emitted bodies (`out[0] = wrap<int>(...)`, `unwrap<double>(in[1])`,
`checkArguments(...)`) compile unchanged. `OutputList` auto-grows on `operator[]`;
`WrapIn = const std::vector<matlab::data::Array>&`. Engine stored in a per-call
global (`gtwrap::setEngine`/`engine()`); `ArrayFactory` built locally per helper.

Only these emitted fragments diverge from the C path:

- handle write to `out[0] = make_handle(self);`
- handle read to `Shared *self = get_handle<Shared>(in[0]);`
- remove `mexAtExit`/`mexLock`/`mexUnlock` (collector `insert/find/erase/delete` kept).
- entry point to `class MexFunction` (strips id, copies `out.v` back to `outputs`).

---

## Stage 0 - Flag plumbing (no behavior change)

- [x] Add `--mex-api {c,cpp}` arg (default `c`) to `scripts/matlab_wrap.py`; assert value in `{'c','cpp'}`.
- [x] Selection is a factory at the call site (Stage 3c): `cpp` -> `MatlabWrapperCpp`, else `MatlabWrapper`. Base class gains no new field.
- [x] Confirm default `c` path leaves every existing test green (pure plumbing).

## Stage 1 - C++ runtime header `matlab_cpp.h`

New file sibling to `matlab.h`, reimplementing every symbol against the Data API.

- [x] Includes: `<mex.hpp>`, `<MatlabDataArray.hpp>`, Eigen/gtsam headers as in `matlab.h:25-48`. `mexAdapter.hpp` is NOT here (only in generated cpp).
- [x] Global engine holder: `gtwrap::setEngine(std::shared_ptr<matlab::engine::MATLABEngine>)` + `gtwrap::engine()` accessor.
- [x] `OutputList` struct (auto-grow `operator[]`) and `using WrapIn = const std::vector<matlab::data::Array>&;`.
- [x] `make_handle<Shared>(Shared*)` to `createScalar<uint64_t>`; `get_handle<Shared>(const Array&)` to `TypedArray<uint64_t>` (ports `matlab.h:522-628` handle encode/decode).
- [x] `integralClassId`, `myGetScalar`, `checkScalar`, `checkArguments` ported to `getType()`/`getDimensions()` (`matlab.h:154-206,383-395`).
- [x] `wrap<T>` specializations: string, char, unsigned char, bool, int, size_t, double (`matlab.h:213-266`).
- [x] `wrap_Vector`/`wrap_Matrix` + `wrap<Vector|Point2|Point3|Matrix>` with row-to-col-major preserved (`matlab.h:269-313`).
- [x] `wrap_enum` via `engine()->feval(u"<classname>", ...)` (`matlab.h:319-331`).
- [x] `unwrap<T>` specializations mirroring all `wrap` types incl. Eigen col-to-row-major (`matlab.h:337-507`).
- [x] `unwrap_enum` via `feval(u"int32", ...)` (`matlab.h:357-369`).
- [x] `create_object`: pointer-key constructor protocol via `feval(classname,{key,ptr[,"void"]})`; RTTI lookup through `getVariable("gtsamwrap_rttiRegistry", GLOBAL)` StructArray (`matlab.h:522-571`). Keep `ptr_constructor_key` byte-identical (`matlab.h:67-75`).
- [x] `wrap_shared_ptr` / `unwrap_shared_ptr` / `unwrap_ptr` via `getProperty` (`matlab.h:578-628`); drop `mexLock`.
- [x] `mstream`/`CoutRedirect` to streambuf calling `engine()->feval(u"fprintf",0,{createScalar(1.0),createCharArray("%s"),createCharArray(text)})` (`matlab.h:83-136`).
- [x] `gtwrap::MexErrMsgTxt` / `MexErrMsgIdAndTxt` / `error` to `restoreActive()` then `feval(u"error", ...)` (`matlab.h:138-152`).

## Stage 2 - Code-gen templates (new `gtwrap/matlab_wrapper/templates_cpp.py`)

Put C++ templates in a new `templates_cpp.py`; leave `templates.py` untouched.

- [x] New file `gtwrap/matlab_wrapper/templates_cpp.py` with `class WrapperTemplateCpp` holding the C++-divergent templates below. The cpp subclass (Stage 3b) imports from here.
- [x] `mex_function`: emits `class MexFunction` (ctor sets engine, dtor calls `_deleteAllObjects`, `operator()` strips id to `in`, switch dispatch, copy `out.v` to `outputs`, same exception text).
- [x] `delete_all_objects`: warning via `engine()->feval` `fprintf`, not `mexPrintf`.
- [x] `rtti_register`: build/read `StructArray` via `getVariable/setVariable(..., GLOBAL)`.
- [x] `collector_function_upcast_from_void`: no `mexAtExit/mexLock`; `out[0] = make_handle(self);`.
- [x] Reuse from `templates.py` (no copy): `typdef_collectors`, serialize/deserialize, `collector_function_shared_return` (all route through `wrap_*`/`unwrap_*`). Only override the four above.

## Stage 3 - Generator: subclass, not in-place branching

Chosen design (option B): keep the C-path generator logic in `wrapper.py`
behaviorally untouched; isolate all C++ divergence in a new subclass. The only
edits to `wrapper.py` are behavior-preserving extractions that turn the ~5
inline divergent fragments into overridable hook methods (C output stays
byte-identical - Stage 7 gate proves it). No fork/duplication of the 2000-line
AST walk.

### 3a. Behavior-preserving extraction in base `wrapper.py` (no output change)

- [x] Extract handle-write fragment (`out[0] = mxCreateNumericMatrix(...); *reinterpret_cast<Shared**>(mxGetData(out[0])) = self;`) from `generate_collector_function` (`:1520-1521,1534-1535,1546-1547`) into `self._emit_handle_write(out_idx, shared_type, expr)`.
- [x] Extract handle-read fragment (`*reinterpret_cast<Shared**>(mxGetData(in[0]))`) (`:1511,1558`) into `self._emit_handle_read(shared_type, in_idx)`.
- [x] Extract lifetime lines into `self._emit_lock()`, `self._emit_unlock()`, `self._emit_atexit()` (`:1509,1522,1536,1541,1545,1566`).
- [x] Extract collector-function signature string (`:1493-1494`) into `self._collector_signature(name)`.
- [x] Extract entry-point selection in `mex_function` (`:1732`) and preamble include line in `generate_preamble` (`:1748+`) into `self._entry_point()` / `self._runtime_include()`.
- [x] Run full suite: C output unchanged (these are pure refactors of the C path).

### 3b. New subclass `gtwrap/matlab_wrapper/wrapper_cpp.py`

- [x] `class MatlabWrapperCpp(MatlabWrapper)` pulling templates from `WrapperTemplateCpp` (Stage 2), overriding only the hooks from 3a to emit C++: `_emit_handle_write` to `make_handle`, `_emit_handle_read` to `get_handle<Shared>`, `_emit_lock/_unlock/_atexit` to empty, `_collector_signature` to `OutputList& out` / `WrapIn in` form, `_entry_point` to `WrapperTemplateCpp.mex_function` (class `MexFunction`), `_runtime_include` to `<wrap/matlab_cpp.h>` + `#include "mexAdapter.hpp"`.
- [x] Override `wrap_collector_function_upcast_from_void` (`:1466-1477`) to use `WrapperTemplateCpp.collector_function_upcast_from_void`.
- [x] Override preamble RTTI/`_deleteAllObjects` template selection to `WrapperTemplateCpp.rtti_register` / `delete_all_objects`.

### 3c. Wire selection

- [x] `scripts/matlab_wrap.py`: `--mex-api cpp` instantiates `MatlabWrapperCpp`, else `MatlabWrapper` (factory at the call site; base keeps no `mex_api` field).

## Stage 4 - CMake (`cmake/MatlabWrap.cmake`) + install

- [x] Add cache option `WRAP_MEX_API` (`c`|`cpp`, default `c`).
- [x] When `cpp`: append `--mex-api cpp` to generator `add_custom_command` (`:253-256`).
- [x] Replace the `-DMX_COMPAT_32` flag block (`:283`) for cpp with `-DMATLAB_DEFAULT_RELEASE=R2021b -DMATLAB_MEX_FILE`, enforce C++17.
- [x] Link `libMatlabDataArray` + `libMatlabEngine` (+ `libmex`) from `${MATLAB_ROOT}/extern/bin/<arch>` across win/mac/linux branches (`:297-322`); keep mex extension + `mexFunction` export.
- [x] Install `matlab_cpp.h` next to `matlab.h` (mirror existing header install rule; check `CMakeLists.txt`).

## Stage 5 - Tests & fixtures (`tests/`)

- [x] Generate `tests/expected/matlab_cpp/<module>_wrapper.cpp` for every module currently under `tests/expected/matlab/`.
- [x] Parametrize each codegen test (`test_geometry`...`test_special_cases`, `test_multiple_files`, `test_templates`, `test_inheritance`, `test_namespaces`, `test_enum`) with `mex_api='cpp'` to compare against new fixtures.
- [x] Assert `.m` outputs from the cpp path are byte-identical to the C path (`.m` reused, not duplicated).
- [x] cpp ownership test (mirror `:389-433`): assert `get_handle`/`make_handle`, collector `find/erase/delete` kept, and absence of `mexLock`/`mexUnlock`/`mxCreateNumericMatrix`.
- [x] cpp scalar-contracts test (mirror `:351-387`): `unwrap< string >(in[1])`, `unwrap< uint32_t >`, no shared-ptr wrap of scalars - unchanged from C.
- [x] Keep `test_non_const_string_ref_is_rejected` passing for both targets.

## Stage 6 - Parity verification checklist

- [x] Collector set per class; ctor insert; dtor stale-check + erase + delete.
- [x] Pointer-key constructor protocol + identical `ptr_constructor_key`.
- [x] RTTI registry global var; virtual upcast (`upcastFromVoid`, `static_pointer_cast`).
- [x] Boost serialize/deserialize collectors emit and route through `wrap_*`.
- [x] All `wrap`/`unwrap` specializations incl. Eigen major-order conversions.
- [x] Scalar-vs-pointer contracts (strings/ints not shared-ptr-wrapped).
- [x] `cout` redirected; exception text `"Exception from wrapped C++ code:\n..."`.
- [x] `.m` files byte-identical between targets.

## Stage 7 - End-to-end verification

- [x] `pytest tests/test_matlab_wrapper.py` green for both `c` and `cpp` targets (primary gate; no MATLAB needed).
- [x] Diff `geometry_wrapper.cpp` C vs cpp manually - only the mapped fragments differ.
- [x] Confirm `--mex-api c` output is bit-identical to current `tests/expected/matlab/` (zero C-path regression).
- [ ] (Optional, needs MATLAB >= R2021b) `cmake -DWRAP_MEX_API=cpp ..`, build MEX, then in MATLAB: construct an object, call a matrix-returning method, trigger an error (verify message), `clear` to confirm `~MexFunction` cleanup.

## Status (prototype complete)

- Codegen + runtime header + CMake done. `pytest tests/` = 102 passed
  (98 prior + 4 new cpp tests); C-path output and `.m` files byte-identical;
  pybind/python unaffected.
- Not yet done: compiling a generated cpp wrapper against a real MATLAB
  >= R2021b toolchain (no MATLAB in this environment). The header follows the
  documented C++ MEX API but is not compiler-verified; a MATLAB build smoke
  test is the one remaining gate.

### Intentional divergences from the C runtime (cleaner, flagged for review)

- `bool` is wrapped as a MATLAB `logical` (was `uint32` in matlab.h) - matches
  the `.m` `data_type` contract and is type-correct.
- `unwrap_ptr` returns `shared_ptr::get()` of the stored handle; matlab.h
  reinterpreted the storage address, which looked like a latent bug for `@`
  raw-pointer args.
- Lifetime: no `mexLock/Unlock/AtExit`; the `MexFunction` instance owns the
  collectors and `~MexFunction()` runs `_deleteAllObjects()`.

## Next improvements the C++ API enables (vs the C API)

1. Drop the engine global: thread a `MATLABEngine`/`ArrayFactory` context object
   through `wrap`/`unwrap` instead of `gtwrap::setEngine`, removing shared
   global state once the generator passes a context parameter.
2. Native error identifiers + stack: throw `matlab::engine::MATLABException`
   instead of string `feval("error", ...)`, surfacing C++ exception type/id to
   MATLAB's `MException` with a real identifier.
3. Zero-copy large data: `ArrayFactory::createArrayFromBuffer` / `buffer_ptr_t`
   to move Eigen matrices in/out without element copies (the C path copies
   element-by-element through `mxGetPr`).
4. Type-checked overload dispatch: use `Array::getType()` to disambiguate
   overloads in C++ with clear mismatch errors, instead of relying solely on the
   `.m` `isa(...)` guards.
5. Interleaved complex + strong typing: `TypedArray<std::complex<double>>` for
   complex Eigen types, impossible cleanly under `-DMX_COMPAT_32`.
6. Simplify the proxy-object pointer-key protocol: with `getProperty`/
   `setProperty` a future version could store handles more directly, shrinking
   `create_object`.

---

# Phase 2 - build verification + roadmap improvements (order 2,3,6,1,4)

## Stage 8 - MATLAB build & run verification [DONE]

- [x] CMake link fix: dropped nonexistent `libMatlabEngine`; the in-process MEX
      engine is provided by `libmex` + `libMatlabDataArray` (all platforms).
- [x] `matlab_cpp.h` compile-fixes surfaced by the first real build:
      add `<set>`/`<map>`; include `mexAdapter.hpp` at the top (via
      `matlab_cpp.h`) so `matlab::mex::ArgumentList` is complete before
      `MexFunction::operator()` on R2023b (R2024b accepts either order).
- [x] New `tests/matlab_runtime/{runtime_demo.h,runtime_demo.i,run_test.m}`:
      gtsam-linked classes exercising scalars, strings, Eigen vector/matrix
      (incl. column-major ordering), enum arg/return, virtual base + derived,
      error propagation, and `clear` lifetime.
- [x] `tests/test_matlab_runtime_build.py`: MATLAB-gated pytest (skips without
      `mex`/gtsam) that, for both MEX APIs, generates the wrapper, compiles with
      `mex` against `libgtsam`, and runs `run_test.m`. Passes on R2023b/R2024b.
- Notes: launching MATLAB needs `LD_PRELOAD` of the system `libstdc++` when the
  build gcc is newer than MATLAB's bundled one (handled in the test). gtwrap
  returns the *declared static type* for base-returning functions (no
  auto-downcast) - both C and C++ paths emit `wrap_shared_ptr(..., false)`.

## Stage 9 - Improvement 2: native MATLABException errors [DONE]

- [x] `MexErrMsgIdAndTxt`/`error` now throw `matlab::engine::MATLABException(id, txt)`
      after restoring cout, instead of `feval("error", ...)`. The MEX framework
      reports it as an MException with both the message and the identifier.
- [x] `MexFunction::operator()` catches and rethrows `MATLABException` (so
      wrap-internal errors keep their id) before the generic `std::exception`
      wrap for genuine C++ exceptions.
- Verified: `run_test.m` asserts `e.identifier == 'wrap:error'`.

## Stage 10 - Improvement 3: bulk Eigen transfer [DONE]

- [x] `unwrap<Vector>`/`unwrap<Matrix>` use a single `std::copy` over the
      read-only column-major iterator instead of per-element `[i][j]` index
      math. The return path was already a single bulk copy (Eigen storage to
      MATLAB), so it is left as `createArray`.
- Verified: matrix/vector value + column-major ordering checks.

## Stage 11 - Improvement 6: proxy internals [DONE]

- [x] `.m` kept byte-identical (pointer-key protocol retained). Factored the
      property to handle extraction into `shared_ptr_handle`, shared by
      `unwrap_shared_ptr` and `unwrap_ptr`.
- Verified: raw-pointer object argument check (`addCount`) exercises
      `unwrap_ptr`.

## Stage 12 - Improvement 1: remove the engine global [DONE]

- [x] Replaced `gtwrap::engine()`/`setEngine` with a per-call `Context{engine,
      factory}`, threaded into the engine-using helpers (shared_ptr/enum/io) via
      a single `_ctx_arg()` generator hook. Basic scalar/Eigen wrap/unwrap and
      `make_handle` need no engine and stay ctx-free. C output stays
      byte-identical (`_ctx_arg()` is empty for the C target).
- `mstream`/`CoutRedirect` take the engine explicitly; RTTI register and
  `_deleteAllObjects` take it as a parameter.
- Bug found by the MATLAB gate and fixed: `getEngine()` returns a fresh
  `shared_ptr`, so the engine must be held in a local for the call/dtor
  duration (a raw pointer from a temporary dangled and segfaulted in
  `setVariable`).

## Stage 13 - Improvement 4: typed mismatch errors [DONE]

- [x] Added `typeName(ArrayType)` + `describe(Array)`; `checkScalar`,
      `myGetScalar`, `unwrap<string|Vector|Matrix>` now report the expected
      vs. actual type/shape.
- Verified: passing a 1x2 to a scalar int arg (the `.m` overload guard checks
      class but not size) yields "expected a scalar, got a 1x2 double array".

## Stage 14 - Final review [DONE]

- Full suite: `pytest tests/` = 103 passed (codegen C + cpp, pybind, and the
  MATLAB build+run gate). C path and `.m` byte-identical; pybind unaffected.
- Known, parity-consistent items (not bugs):
  - The `isVirtual=true` path (`create_object` RTTI read) is never emitted by
    the generator - both C and C++ always pass `false`; gtwrap returns the
    declared static type. The RTTI registry is written but only read on that
    unused path.
  - `-Wextra` reports unused `ctx`/`nargout`/`out`/`nargin`/`in` in collectors
    that do not need them; identical in spirit to the C path's unused
    `nargout`/`out`/etc. Default mex flags are warning-clean.
  - Building with a system g++ newer than MATLAB's bundled libstdc++ needs
    `LD_PRELOAD` of the system libstdc++ (handled by the test).

## Addendum - C/C++ runtime parity and teardown safety

The MATLAB build+run gate (`tests/test_matlab_runtime_build.py`) is
parametrized over both MEX APIs. The same `run_test.m` runs against the C and
the C++ build; an `api` argument gates the three C++-target-only refinements
(bool returned as `logical`, the `wrap:error` identifier, and type/shape-named
mismatch messages), so the shared functional surface is asserted identically on
both.

`tests/matlab_runtime/run_cleanup.m` stresses teardown for both targets:

- `reload`: use the wrapper, `clear all` (which tears the module down and, for
  the C++ target, runs `~MexFunction` / `_deleteAllObjects` over live collector
  entries), then construct and use brand-new objects. No segfault on either
  target; fresh objects work after the teardown.
- `exit`: leave live objects in the collector and let the `-batch` session end,
  so MATLAB unloads the module on exit. No segfault on either target.

Bug found and fixed in the process: the legacy C runtime's `unwrap_ptr` returned
a pointer into the handle storage instead of the wrapped object (raw-pointer
`Type@` arguments). Details in `bug-report-c-unwrap-ptr.md`. The C++ runtime was
correct already. Fix is a one-liner in `matlab.h`; no generated code or fixtures
change.

---

# Phase 3 - runtime correctness and performance follow-up

## Stage 15 - C enum temporary cleanup [DONE]

- [x] Fix legacy C `wrap_enum` / `unwrap_enum` temporary `mxArray` ownership so
      repeated enum calls do not leak the conversion arrays.
- [x] Validate with the MATLAB wrapper codegen suite and the MATLAB runtime
      build gate.

## Stage 16 - C++ destructor safety [DONE]

- [x] Make generated C++ `~MexFunction` cleanup best-effort and non-throwing in
      practice, so `clear mex` / MATLAB shutdown cannot terminate if engine-side
      cleanup printing fails.
- [x] Regenerate C++ expected wrapper fixtures and validate the C++ codegen and
      runtime cleanup gates.

## Stage 17 - C++ fixed-overhead reduction [DONE]

- [x] Replace the copied C++ input vector with a zero-copy `WrapIn` view over
      `matlab::mex::ArgumentList`.
- [x] Cache RTTI registration per generated `MexFunction` instance so every
      method call does not hit the global workspace.
- [x] Add codegen assertions for the zero-copy input view and one-time RTTI
      registration.

## Stage 18 - review gate and benchmark report [DONE]

- [x] Review the diff before benchmarking: C output and `.m` output must stay
      stable; C++ generated output should only reflect destructor/input-view/RTTI
      changes.
- [x] Run the full post-fix benchmark and update
      `c-vs-cpp-mex-benchmark.md` with the new numbers, raw output path, and
      corrected `noop` interpretation.
- [x] Close this phase on correctness, teardown safety, and removal of avoidable
      overhead. The C++ backend is not required to be faster than the C backend;
      the benchmark informs the recommendation rather than acting as a speed
      pass/fail gate.
