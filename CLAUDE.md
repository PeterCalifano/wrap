# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

`wrap` (published as the `gtwrap` package) is a tool that wraps C++ code into Python bindings (via Pybind11) and MATLAB toolboxes. It was originally developed for GTSAM but designed to be general-purpose.

The two wrapping targets are:
- **Python/Pybind11**: Parses an interface `.h` file and generates `.cpp` Pybind11 binding code
- **MATLAB**: Parses an interface `.h` file and generates MEX wrapper code

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

Python tests use pytest (configured in `pyproject.toml`):
```sh
# Run all tests from project root
pytest tests/

# Run a single test file
pytest tests/test_interface_parser.py

# Run with coverage (default from pyproject.toml addopts)
pytest
```

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

- **`gtwrap/pybind_wrapper.py`** (`PybindWrapper` class): Walks the instantiated AST and generates Pybind11 `.cpp` binding code
- **`gtwrap/matlab_wrapper/wrapper.py`** (`MatlabWrapper` class): Walks the instantiated AST and generates MATLAB MEX wrapper code
- **`gtwrap/matlab_wrapper/mixins.py`**: Shared logic between wrapper classes (e.g., pointer/shared_ptr detection)

### CMake Integration

- **`cmake/PybindWrap.cmake`**: Defines `pybind_wrap()` CMake function that invokes `scripts/pybind_wrap.py` as a custom command
- **`cmake/MatlabWrap.cmake`**: Defines `wrap_and_install_library()` CMake function that invokes `scripts/matlab_wrap.py`
- **`cmake/configure_wrap_paths.cmake`**: Alternative to `find_package(gtwrap)` for use without installation — sets `GTWRAP_PYTHON_PACKAGE_DIR` and includes the CMake scripts directly
- **`cmake/gtwrapConfig.cmake.in`**: Template for the installed package config

### Entry Points

- `scripts/pybind_wrap.py` — CLI entry point invoked by CMake for Python wrapping
- `scripts/matlab_wrap.py` — CLI entry point invoked by CMake for MATLAB wrapping

### Key Files

| Path | Purpose |
|------|---------|
| `matlab.h` | C++ header required by generated MATLAB wrappers |
| `templates/matlab_wrapper.tpl.in` | Template for generated MATLAB wrapper `.tpl` file |
| `templates/pybind_wrapper.tpl.example` | Example template for generated Pybind11 module |
| `pybind11/` | Bundled pybind11 submodule |
| `gtwrap/xml_parser/xml_parser.py` | Parses Doxygen XML to add docstrings to Python bindings |

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
