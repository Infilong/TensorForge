# Compiler Optimization Experiment

## Environment

Clang 22.1.8, targeting `x86_64-w64-windows-gnu`.

## Findings

### Dead-code elimination

An arithmetic loop whose result was never observed compiled to `retq` under
optimized builds.

### Reduction optimization

A runtime-sized integer summation loop was transformed into constant-time
arithmetic by Clang.

### Auto-vectorization

A float elementwise-add loop was vectorized at `-O2` and `-O3`.

Clang reported:

```text
vectorization width: 4
interleaved count: 2
```

Generated code included packed float instructions (`addps`).

## Benchmark caveat

Debug vs Release timing is not an isolated SIMD comparison because Debug also
contains sanitizer instrumentation and disables normal optimization.
