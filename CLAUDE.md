# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

> A near-identical `AGENTS.md` exists for Codex. If you change the shared
> sections here (build, test, architecture), mirror the change there.

## Project Overview

`wrap` (published as the `gtwrap` package) is a tool that wraps C++ code into Python bindings (via Pybind11) and MATLAB toolboxes. It was originally developed for GTSAM but designed to be general-purpose.

The two wrapping targets are:

- **Python/Pybind11**: Parses an interface `.h` file and generates `.cpp` Pybind11 binding code
- **MATLAB**: Parses an interface `.h` file and generates MEX wrapper code. The MATLAB target has **two interchangeable backends** (see below): the legacy C MEX API and the modern C++ MEX API.

## Build & Install

```sh
# Standard CMake build (installs CMake scripts and Python package)
mkdir build && cd build
cmake ..
make install  # use sudo if needed
```

**Prerequisite:** Install `pyparsing` before building:

```sh
python3 -m pip install pyparsing
```

**For use without install** (from source), include `cmake/configure_wrap_paths.cmake` instead of using `find_package(gtwrap)`.

## Running Tests

Python tests use pytest (configured in `pyproject.toml`, which sets coverage on `gtwrap` by default):

```sh
# Run all tests from project root
pytest tests/

# Run only the MATLAB codegen tests (the main gate; no MATLAB needed)
pytest tests/test_matlab_wrapper.py

# Run a single test file / single test
pytest tests/test_interface_parser.py
pytest tests/test_matlab_wrapper.py::TestWrapCpp::test_cpp_modules_match_expected
```

`tests/test_matlab_runtime_build.py` is a **MATLAB-gated** end-to-end test: it
generates a wrapper, compiles it with the MATLAB `mex` compiler against real
`libgtsam`, and runs `.m` drivers. It auto-skips when `matlab`/`mex`/gtsam are
not present, so it is a no-op in CI and most dev machines.

CI (`.github/workflows/`): `matlab-tests.yml` runs the pytest suite;
`linux-ci.yml` / `macos-ci.yml` are the upstream build matrices.

## Architecture

### Parsing Pipeline

The wrapping pipeline has two main stages:

1. **Interface Parser** (`gtwrap/interface_parser/`): Parses C++ interface header files (`.h` or `.i`) using `pyparsing`. The grammar produces an AST of modules, namespaces, classes, methods, constructors, enums, functions, and types. Key modules:
   - `module.py` - top-level container (a parsed `.h` file)
   - `namespace.py` - namespace blocks
   - `classes.py` - class definitions including methods, constructors, properties
   - `type.py` - type representations (including shared_ptr, raw ptr, templates)
   - `function.py` - free function definitions
   - `template.py` - template parameter handling

2. **Template Instantiator** (`gtwrap/template_instantiator/`): Takes the parsed AST and expands template instantiations into concrete types. Produces a new AST suitable for code generation.

### Code Generators

- **`gtwrap/pybind_wrapper.py`** (`PybindWrapper`): generates Pybind11 `.cpp` binding code.
- **`gtwrap/matlab_wrapper/wrapper.py`** (`MatlabWrapper`): the ~2000-line AST walk that generates MATLAB MEX wrapper code. This is the **legacy C MEX API** generator and the default.
- **`gtwrap/matlab_wrapper/wrapper_cpp.py`** (`MatlabWrapperCpp(MatlabWrapper)`): the **modern C++ MEX API** generator. It is a subclass, NOT a fork — it overrides only a handful of hook methods.
- **`gtwrap/matlab_wrapper/templates.py`** (`WrapperTemplate`) and **`templates_cpp.py`** (`WrapperTemplateCpp`): the static code fragments for the C and C++ targets respectively.
- **`gtwrap/matlab_wrapper/mixins.py`**: shared logic between wrapper classes (e.g., pointer/shared_ptr detection, type recognition helpers).

### The two MATLAB MEX backends (important)

The MATLAB target can emit against either MEX API, selected at codegen time:

- `scripts/matlab_wrap.py --mex-api {c,cpp}` (default `c`). The script picks
  `MatlabWrapperCpp` for `cpp`, else `MatlabWrapper`.
- CMake: cache option `WRAP_MEX_API` (`c`|`cpp`, default `c`) in
  `cmake/MatlabWrap.cmake`; the `cpp` path adds the flag, links the MATLAB Data
  Array libs, and enforces C++17 / `R2021b`.
- Runtime headers, included by the generated `.cpp`: `matlab.h` (C API) and
  `matlab_cpp.h` (C++ API). Both must stay at feature parity.

**Design contract — read before touching either MATLAB generator:**

1. `MatlabWrapperCpp` is a subclass. All C++ divergence is isolated in a small
   set of overridable hooks (`_emit_handle_write`, `_emit_handle_read`,
   `_emit_lock/_unlock/_atexit`, `_collector_signature`, `_entry_point`,
   `_runtime_include`, `_ctx_arg`, etc.). Do not branch on the target inside the
   base AST walk; add/override a hook instead.
2. **The C path output is frozen.** Any change to `wrapper.py` must leave
   `tests/expected/matlab/*.cpp` byte-identical (refactors there must be pure
   extractions). The golden files are the gate.
3. **The generated `.m` classdef files are byte-identical between targets** —
   only the generated `_wrapper.cpp` and the runtime header differ. Both targets
   expose the same `module_wrapper(id, this, args...)` callable and the
   pointer-key constructor protocol (`ptr_constructor_key`).

Background and the full C-vs-C++ API mapping live in
`docs/development/cpp-mex-api-expansion.md`. Other planning/design notes are in
`docs/development/` (e.g. `extended-type-support.md`,
`bug-report-c-unwrap-ptr.md`) — consult them before extending type support or
the runtime headers.

### Tests for the MATLAB targets

- **Golden fixtures**: `tests/expected/matlab/<module>_wrapper.cpp` (C target)
  and `tests/expected/matlab_cpp/<module>_wrapper.cpp` (C++ target). Generated
  from `tests/fixtures/*.i`. When generator output legitimately changes,
  regenerate BOTH sets. `TestWrap` covers the C target, `TestWrapCpp` the C++
  target (in `tests/test_matlab_wrapper.py`).
- The cpp tests also assert the `.m` output is byte-identical to the C path and
  that the cpp `.cpp` uses the safe handle API (`get_handle`/`make_handle`, no
  `mexLock`/`mxCreateNumericMatrix`).

### CMake Integration

- **`cmake/PybindWrap.cmake`**: `pybind_wrap()` invokes `scripts/pybind_wrap.py`.
- **`cmake/MatlabWrap.cmake`**: `wrap_and_install_library()` invokes `scripts/matlab_wrap.py`; honours `WRAP_MEX_API`.
- **`cmake/configure_wrap_paths.cmake`**: alternative to `find_package(gtwrap)` for use without installation — sets `GTWRAP_PYTHON_PACKAGE_DIR` and includes the CMake scripts directly.
- **`cmake/gtwrapConfig.cmake.in`**: template for the installed package config.

### Entry Points

- `scripts/pybind_wrap.py` — CLI entry point invoked by CMake for Python wrapping
- `scripts/matlab_wrap.py` — CLI entry point invoked by CMake for MATLAB wrapping (`--mex-api {c,cpp}`)

### Key Files

| Path | Purpose |
|------|---------|
| `matlab.h` | C MEX-API runtime header included by generated C-target wrappers |
| `matlab_cpp.h` | C++ MEX-API runtime header included by generated cpp-target wrappers |
| `templates/matlab_wrapper.tpl.in` | Template for the generated MATLAB wrapper `.tpl` file |
| `templates/pybind_wrapper.tpl.example` | Example template for the generated Pybind11 module |
| `pybind11/` | Bundled pybind11 submodule |
| `gtwrap/xml_parser/xml_parser.py` | Parses Doxygen XML to add docstrings to Python bindings |

## Conventions

- **ASCII-clean generated code and runtime headers**: no em-dashes, unicode
  arrows, or smart quotes in `matlab.h`, `matlab_cpp.h`, templates, or emitted
  code. Use plain ASCII (`->`, `--`, `"`).
- Match the parity invariants above: never regress the frozen C output or the
  shared `.m` files.

## Interface File Syntax Rules

Interface files (`.h` or `.i`) use a C++-like syntax. Key constraints:

- Classes must start with uppercase
- Only one method/constructor per line
- All namespaces must be fully qualified in arguments and return types
- Includes must use angle brackets: `#include <path>`
- Templates: explicit instantiation list `template<T = {Type1, Type2}>` or typedef
- Virtual classes need `virtual` keyword on both base and derived
- Virtual classes must have a `clone()` method in C++
- Use `@` for raw pointers (`Pose3@`) and `*` for shared pointers (`Base*`)
- Operator overloading (Python only) must be marked `const`
