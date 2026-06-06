# Plan: extend MATLAB wrapper type support (typed Eigen, std::vector, float)

## Context

The MATLAB target today supports only double-precision Eigen (`gtsam::Vector`,
`gtsam::Matrix`) and a fixed scalar set (no `float`); a `std::vector<primitive>`
falls through to opaque `shared_ptr` wrapping rather than a MATLAB array. The
Pybind/Python target already handles all of these automatically via
`pybind11/eigen.h` + `stl.h`, so this closes a MATLAB-vs-Python gap.

Goal: extend the MATLAB generator and both runtime headers (`matlab.h` legacy C,
`matlab_cpp.h` modern C++) to support typed Eigen `Eigen::Matrix<T, R, C>`,
numeric `std::vector<T>` (MATLAB column vector), `std::vector<std::string>`
(MATLAB cellstr), and `float` as a standalone scalar, on both MEX targets, at
parity, without disturbing the existing `gtsam::Vector`/`Matrix` (double) path.

Decisions (from user):

- Both C and C++ MEX targets, at parity.
- Typed Eigen spelled `Eigen::Matrix<T, R, C>` (R/C literals or `Eigen::Dynamic`;
  a column vector is `Eigen::Matrix<T, R, 1>`). Plain `Vector`/`Matrix` stay
  gtsam double.
- Element (scalar) types: all with a MATLAB class - double, single (`float`),
  int8/uint8/int16/uint16/int32/uint32/int64/uint64, `int` -> int32,
  `size_t` -> uint64, `bool` -> logical.
- Plus `float` standalone scalar and `std::vector<std::string>` -> cellstr.
- NOT `std::vector<ClassType>`.

## How the AST exposes these

`Eigen::Matrix<int,3,3>` parses to a `TemplatedType`: `typename.name=='Matrix'`,
`typename.namespaces==['Eigen']`, `template_params==[Type(int),Type(3),Type(3)]`,
`to_cpp()` -> `Eigen::Matrix<int, 3, 3>`. `std::vector<double>` ->
`typename.name=='vector'`, `'std' in namespaces`, `template_params==[Type(double)]`.
The generator currently keys only on `typename.name` and ignores the params,
which is why typed Eigen is treated as double and `std::vector` as an object.

---

## Stage 0 - Recognition helpers (no emitted-output change yet)

- [ ] In `gtwrap/matlab_wrapper/mixins.py` add `eigen_scalar(self, ctype)`:
      return the scalar C++ string when `ctype` is a `TemplatedType` with
      `typename.name=='Matrix'` and `'Eigen' in typename.namespaces`, else None.
- [ ] Add `eigen_is_colvector(self, ctype)`: True when the 3rd template param is
      the literal `1` (so it maps to an `Nx1` MATLAB guard).
- [ ] Add `std_vector_element(self, ctype) -> Optional[tuple]`: `(elem_cpp, kind)`
      where `kind in {'numeric','string'}` when `typename.name=='vector'` and
      `'std' in namespaces` and the single param is a primitive/string; return
      None for object element types (keep today's shared_ptr behavior).
- [ ] Add a scalar-name -> MATLAB-class lookup used by guards and element typing
      (reuse/extend `data_type_param`); include `'float' -> 'single'`.
- [ ] Add `'float': 'single'` to `data_type` and `data_type_param` in
      `gtwrap/matlab_wrapper/wrapper.py`.
- [ ] Quick check: import + call helpers on a parsed `Eigen::Matrix<int,3,3>`
      and `std::vector<double>` to confirm detection; run full `pytest tests/`
      to confirm no output changed (helpers unused so far).

## Stage 1 - Runtime headers (both `matlab.h` and `matlab_cpp.h`)

These helpers use only the array factory (no engine), so the C++ ones need NO
`Context` and slot into the same ctx-free path as `wrap<double>`.

- [ ] `matlab_cpp.h`: `wrap_eigen<Derived>(const Eigen::MatrixBase<Derived>&)`
      -> `Array` using `S = Derived::Scalar`, `createArray<S>`, column-major
      direct copy.
- [ ] `matlab_cpp.h`: `unwrap_eigen<EigenT>(const Array&)` -> `EigenT`; validate
      `array.getType()` equals the MATLAB class of `EigenT::Scalar` (clear
      `typeName`/`describe` error otherwise), bulk `std::copy` into `EigenT`.
- [ ] `matlab_cpp.h`: `wrap_std_vector<T>(const std::vector<T>&)` (numeric
      column array) and `unwrap_std_vector<std::vector<T>>(const Array&)`.
- [ ] `matlab_cpp.h`: `wrap_cellstr(const std::vector<std::string>&)` ->
      `CellArray` of `CharArray`, and `unwrap_cellstr(const Array&)` ->
      `std::vector<std::string>`.
- [ ] `matlab_cpp.h`: `wrap<float>` / `unwrap<float>` (single; `myGetScalar`
      already reads `ArrayType::SINGLE`).
- [ ] `matlab.h`: same surface via the C API - `classIdOf<S>()`
      (double->`mxDOUBLE_CLASS`, float->`mxSINGLE_CLASS`, ints via
      `integralClassId<S>`), `mxCreateNumericMatrix` + `mxGetData` cast to `S*`
      for `wrap_eigen`/`unwrap_eigen`/`wrap_std_vector`/`unwrap_std_vector`;
      `mxCreateCellMatrix` + `mxCreateString` for cellstr; `wrap<float>` /
      `unwrap<float>`.
- [ ] Keep the existing `gtsam::Vector`/`Matrix` (double) specializations intact;
      only typed `Eigen::Matrix<...>` uses the new generic path.

## Stage 2 - Generator emission + `.m` guards (`wrapper.py`)

- [ ] `_unwrap_argument` (~407-454): before the `can_be_pointer`/`else` cases,
      add: eigen-typed -> `unwrap_eigen< <to_cpp> >(in[id])`; numeric
      `std::vector` -> `unwrap_std_vector< <to_cpp> >(in[id])`; string
      `std::vector` -> `unwrap_cellstr(in[id])`. Plain `float` uses the generic
      `unwrap< float >` once Stage 1 lands.
- [ ] `_collector_return` (~1339) and `wrap_collector_function_return_types`
      (~1306): emit `wrap_eigen(obj)` / `wrap_std_vector(obj)` /
      `wrap_cellstr(obj)`; plain float via `wrap< float >`.
- [ ] `.m` overload guards: `_wrap_variable_arguments` (~287-330) and the
      Vector/Point size-check block (~316-328, 385-397): eigen scalar -> MATLAB
      class + an `Nx1`/`size(...,2)==1` guard when it is a column vector; numeric
      `std::vector` -> element MATLAB class + a 1-D guard; string `std::vector`
      -> `'cell'`; float -> `'single'`.
- [ ] Confirm the new emissions carry NO `ctx` (factory-only), so the cpp
      subclass inherits them unchanged.
- [ ] Run `pytest tests/`: existing C and cpp fixtures must stay byte-identical
      (no typed types appear in them).

## Stage 3 - Codegen tests + golden fixtures

- [ ] New `tests/fixtures/typed_types.i`: a class with methods covering
      `Eigen::Matrix<int,2,2>`, `Eigen::Matrix<float,3,1>`,
      `Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>`, `std::vector<double>`,
      `std::vector<int>`, `std::vector<string>`, and a `float` arg/return.
- [ ] Generate goldens: `tests/expected/matlab/typed_types_wrapper.cpp` and
      `tests/expected/matlab_cpp/typed_types_wrapper.cpp` (+ the `.m` files).
- [ ] Add a `test_typed_types` to `tests/test_matlab_wrapper.py` (C target) and
      cover it in the `TestWrapCpp` module list (cpp target), plus targeted
      assertions: `unwrap_eigen<`, `wrap_std_vector(`, `unwrap_cellstr(`,
      `isa(...,'int32')`/`'single'`/`'cell'` in the `.m`.

## Stage 4 - MATLAB runtime gate (both APIs, real libgtsam)

- [ ] Extend `tests/matlab_runtime/runtime_demo.{h,i}` with: an
      `Eigen::Matrix<int,2,2>` round-trip method, an `Eigen::Matrix<float,3,1>`
      round-trip, a `std::vector<double>` and `std::vector<int>` sum/echo, a
      `std::vector<string>` echo, and a `float` scalar echo.
- [ ] Extend `tests/matlab_runtime/run_test.m` with checks asserting class
      (`int32`/`single`), values, and `(i,j)` column-major ordering for the
      typed Eigen; vector length + values; cellstr contents; float value.
- [ ] Run `tests/test_matlab_runtime_build.py` (builds + runs both C and cpp in
      MATLAB). Iterate on any compiler/runtime issues (e.g. Eigen fixed-size
      `EigenT(rows,cols)` assertions, `TypedArray<size_t/bool>` coverage).

## Stage 5 - Docs + final review

- [ ] Update `DOCS.md` supported-types section (typed Eigen, numeric
      `std::vector`, cellstr, float) for the MATLAB target.
- [ ] Record the change in this file (`docs/development/extended-type-support.md`).
- [ ] Final sweep: ASCII-clean (no em-dashes/arrows/smart quotes), comments
      human-readable, C path and pybind unaffected outside the new emissions.

## Verification

- `pytest tests/`: codegen goldens (existing C + cpp unchanged; new typed
  fixtures added) and pybind unaffected.
- MATLAB build gate for both APIs (`tests/test_matlab_runtime_build.py`):
  `Eigen::Matrix<int,2,2>` and `Eigen::Matrix<float,3,1>` round-trips (class +
  ordering), `std::vector<double>`/`<int>` (values + length),
  `std::vector<string>` (cellstr), and a `float` scalar.
- Confirm plain `gtsam::Vector`/`Matrix` output and the C path stay
  byte-identical outside the new typed emissions.

## Risks / confirm during build

- Eigen fixed-size unwrap: `EigenT(rows, cols)` asserts dimensions for fixed
  sizes; validate through the build gate (and accept dynamic forms).
- `TypedArray<S>` / `createArray<S>` coverage for `size_t` (uint64) and `bool`.
- Element-type policy: the `.m` `isa` guard enforces the exact MATLAB class, so
  `unwrap_eigen`/`unwrap_std_vector` require the matching type and error (via
  `typeName`/`describe`) rather than silently converting.

## Critical files

- `gtwrap/matlab_wrapper/mixins.py` (recognition helpers)
- `gtwrap/matlab_wrapper/wrapper.py` (emission, `.m` guards, `data_type` float)
- `matlab.h`, `matlab_cpp.h` (runtime helpers, float scalar)
- `tests/fixtures/typed_types.i`, `tests/expected/matlab*/typed_types_wrapper.cpp`,
  `tests/test_matlab_wrapper.py` (codegen)
- `tests/matlab_runtime/{runtime_demo.h,runtime_demo.i,run_test.m}`,
  `tests/test_matlab_runtime_build.py` (runtime gate)
- `DOCS.md`, `docs/development/extended-type-support.md`
