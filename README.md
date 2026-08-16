# TensorForge

TensorForge is an educational, production-style C++/CUDA AI inference runtime.
The project is intended to support focused experiments in systems programming,
parallel computing, GPU architecture, and performance engineering.

## Current milestone

Milestone 0.1 establishes only the C++ project foundation. It does not yet
contain tensor operations, CUDA code, or third-party dependencies.

## Build

The project requires CMake 3.20 or newer, Ninja, and a C++20 compiler. CMake
presets keep generated files in a separate directory for each configuration.

### Debug

```sh
cmake --preset debug
cmake --build --preset debug
```

### Release

```sh
cmake --preset release
cmake --build --preset release
```

### Release with debug information

```sh
cmake --preset relwithdebinfo
cmake --build --preset relwithdebinfo
```

Executables are generated under the matching `build/debug`, `build/release`,
or `build/relwithdebinfo` directory. Add `.exe` when running on Windows.

To enable AddressSanitizer and UndefinedBehaviorSanitizer with GCC or Clang,
add `-DTENSORFORGE_ENABLE_SANITIZERS=ON` after the configure preset command.
