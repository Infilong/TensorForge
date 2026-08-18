# TensorForge

TensorForge is an educational, production-style C++/CUDA AI inference runtime.
The project is intended to support focused experiments in systems programming,
parallel computing, GPU architecture, and performance engineering.

## Current milestone

Milestone 0.2 adds a focused compiler-optimization experiment to the C++
project foundation. The project does not yet contain tensor operations, CUDA
code, or third-party dependencies.

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

The Debug preset enables AddressSanitizer and UndefinedBehaviorSanitizer with
GCC or Clang. Release and RelWithDebInfo disable sanitizers.

## Compiler-optimization experiment

The `tensorforge_compiler_optimization` executable compares unobservable work,
an observable integer sum, and a simple float elementwise addition loop. Its
optional argument is the runtime element count.

```sh
cmake --preset debug
cmake --build --preset debug --target tensorforge_compiler_optimization
./build/debug/tensorforge_compiler_optimization 5000000
```

Replace `debug` with `release` or `relwithdebinfo` to compare configurations.
On Windows, run `tensorforge_compiler_optimization.exe`. Timings are illustrative
observations, not benchmark-quality performance claims.
