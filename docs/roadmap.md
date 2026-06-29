# VDX Roadmap

## v0.1.x — Stabilization & Math & Graph & AI Start

**Focus:** Bug fixes, math module expansion, graph module completion, AI module foundation.

### Bug Fixes
- Continue hardening the interpreter — edge cases in scoping, type checking, error reporting
- Improve error messages with better context and suggestions
- Fix any remaining `this` / class / import edge cases

### Math Module Expansion
- Add more math functions: `math.log`, `math.exp`, `math.log10`, `math.pow` improvements
- Add `math.gcd`, `math.lcm` for integer utilities
- Add `math.degrees`, `math.radians` for angle conversions
- Add more constants: `math.e`, `math.inf`, `math.nan`
- Improve `math.random` with float support and seed control

### Graph Module Completion
- Add plot styling: colors, markers, line styles
- Add multiple series on the same plot
- Add grid lines and legends
- Add PNG export option
- Cross-platform `graph.show()` (currently Windows-only)
- Add `graph.xlabel` / `graph.ylabel` / `graph.title` persistence across plots

### AI Module — Initial Foundation
- Start `ai` module with basic tensor data structure
- Basic tensor operations: create, shape, reshape, transpose
- Element-wise arithmetic on tensors
- Matrix multiplication
- Random tensor initialization (normal, uniform)

---

## v0.2.x — AI & Tensors

**Focus:** AI module core, tensor operations, neural network primitives.

### Tensor System
- Full tensor implementation with arbitrary dimensions
- Broadcasting for element-wise operations
- Tensor indexing and slicing
- Tensor concatenation and stacking
- Reduction operations: `sum`, `mean`, `max`, `min` along axes

### Neural Network Primitives
- Activation functions: `relu`, `sigmoid`, `tanh`, `softmax`
- Loss functions: `mse`, `cross_entropy`
- Gradient computation and backpropagation
- Optimizers: SGD, Adam
- Layer abstractions: linear, dense, sequential

### Data Utilities
- CSV / data loading into tensors
- Train / test split helpers
- Batch iteration utilities
- Normalization and standardization helpers

---

## v0.3.x — AI Completion & Benchmarks

**Focus:** Finish AI module, add benchmarking module.

### AI Module — Finalization
- Pre-trained model loading / saving
- Inference helpers
- Model evaluation metrics
- Training loop utilities with epoch tracking
- Example models: linear regression, simple classifier

### Benchmark Module
- `bench.time(fn)` — measure execution time of a function
- `bench.repeat(fn, n)` — run n times, report avg / min / max
- `bench.compare(fn1, fn2)` — side-by-side comparison
- Memory usage tracking
- Results export to CSV or JSON
- Integration with `graph` module for visualizing benchmark results
