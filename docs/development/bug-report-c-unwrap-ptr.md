# Bug report: C MEX API `unwrap_ptr` returns a garbage pointer

## Status

Found by the new C-target runtime test (`tests/matlab_runtime/run_test.m`,
`raw-pointer object argument` check) and fixed in `matlab.h`. Recorded here
because it was a latent correctness bug in the shipped C MEX runtime.

## Summary

`unwrap_ptr<Class>` in `matlab.h` is used whenever a wrapped method or function
takes a raw-pointer argument (interface syntax `Type@`). It returned a pointer
into the MATLAB handle storage instead of the wrapped C++ object, so the callee
received a garbage `Class*`. Depending on use this produced wrong results or a
crash.

## Root cause

Every wrapped object stores, in its `ptr_<Class>` property, a heap-allocated
`std::shared_ptr<Class>*` (set by the constructor collectors as
`*reinterpret_cast<Shared**>(mxGetData(out[0])) = self;`). The sibling
`unwrap_shared_ptr` reads it correctly:

```cpp
std::shared_ptr<Class>* spp =
    *reinterpret_cast<std::shared_ptr<Class>**>(mxGetData(mxh));
return *spp;
```

`unwrap_ptr` instead reinterpreted the address of the storage as the object:

```cpp
Class* x = reinterpret_cast<Class*>(mxGetData(mxh));  // wrong: &storage, not the object
return x;
```

`mxGetData(mxh)` points at the `uint64` slot holding the `shared_ptr<Class>*`
value, not at the C++ object. Casting that address to `Class*` yields a pointer
into MEX-managed memory.

## Fix

Dereference the stored `shared_ptr<Class>*` and return its `.get()`, mirroring
`unwrap_shared_ptr`:

```cpp
std::shared_ptr<Class>* spp =
    *reinterpret_cast<std::shared_ptr<Class>**>(mxGetData(mxh));
if (!spp)
  error("Parameter is not a pointer type: null shared pointer.");
return spp->get();
```

## Impact and scope

- Affects only the legacy C MEX API path; the C++ MEX API runtime (`matlab_cpp.h`)
  was implemented correctly from the start (`shared_ptr_handle(...)->get()`).
- Triggered by any wrapped signature with a `Type@` raw-pointer argument.
- No generated code or test fixture changes: `unwrap_ptr` is a runtime-header
  function; generated wrappers call it identically. The C codegen golden tests
  remain byte-identical.

## Verification

`tests/test_matlab_runtime_build.py` builds `runtime_demo` for both MEX APIs and
runs `run_test`. `demo::Counter::addCount(const Counter* other)` (interface
`int addCount(demo::Counter@ other) const;`) exercises `unwrap_ptr`:

- Before: `raw-pointer object argument` check failed on the C target.
- After: passes on both the C and C++ targets.
