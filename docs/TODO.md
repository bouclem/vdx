# VDX TODO

## v0.0.1
- [x] Lexer (tokenizer)
- [x] Parser (AST)
- [x] Interpreter
- [x] `class` support
- [x] `print()` support
- [x] Example program

## v0.0.2
- [x] Variables (`let`)

## v0.0.3
- [x] Functions (`fn`)
- [x] Operators (+, -, *, /, ==, !=, <, >, <=, >=)

## v0.0.4
- [x] `this` keyword
- [x] `if` / `elif` / `else`
- [x] Inline expression evaluation in `print()`

## v0.0.5
- [x] `while` loop
- [x] Block scoping
- [x] `wait(ms)`
- [x] Variable reassignment (`name = expr;`)

## v0.0.6
- [x] Loop safety protection (blocks while loops iterating < 2s)
- [x] `@unsafe` annotation to bypass loop protection
- [x] SVG icon (`assets/icon.svg`)
- [x] Windows MSI installer (CPack + WiX)
- [x] Installer adds `vdx` to system PATH
- [x] Website v0.1.0 (main page, download page, docs)

## v0.0.7
- [x] Arrays / lists (`let arr = [1, 2, 3];`)
- [x] Array index access (`arr[0]`)
- [x] Array index assignment (`arr[0] = 5;`)
- [x] String index access (`str[0]`)
- [x] Built-in `len()` for arrays and strings
- [x] Built-in `push()` to append to arrays
- [x] Improved error reporting (source context, line numbers, file name)

## v0.0.8
- [x] Types: `float` literals (`3.14`), `true`/`false` boolean literals
- [x] Optional type annotations: `let x: int = 5;`, `let pi: float = 3.14;`, `let name: string = "hi";`, `let ok: bool = true;`
- [x] Runtime type checking on annotated variables
- [x] Mixed int/float arithmetic (auto-promotion to float)
- [x] `new` / object instantiation: `let obj = new ClassName();`
- [x] Dot field access: `obj.field`
- [x] Dot field assignment: `obj.field = value;`
- [x] Dot method calls: `obj.method(args)`
- [x] C-style `for` loop: `for (let i = 0; i < n; i = i + 1) { ... }`
- [x] `for-in` loop over arrays: `for (item in arr) { ... }`
- [x] `@unsafe` support for `for` loops

## v0.0.9
- [x] `break` statement for exiting loops early
- [x] `continue` statement for skipping loop iterations
- [x] `const` keyword for immutable variables
- [x] Const type annotations: `const PI: float = 3.14;`
- [x] `math` module with: sqrt, pow, abs, sin, cos, tan, floor, ceil, round, min, max, random, pi
- [x] `vdx/src/modules/` directory structure
- [x] Loop control exception handling

## v0.0.10
- [x] Modulo operator `%`
- [x] Increment `++` and decrement `--` operators
- [x] MSI installer PATH option

## v0.0.11
- [x] `import` statement: import other VDX files
- [x] Circular import protection
- [x] `type(value)` built-in: returns type as string
- [x] `input()` built-in: read user input
- [x] `input(prompt)` variant
- [x] `pop(arr)` built-in: remove and return last element
- [x] `len(obj)` extension for objects

## v0.0.12
- [x] Dictionary/map type: `let user = {"name": "Alice"};`
- [x] Dictionary access: `user["key"]`
- [x] Dictionary assignment: `user["key"] = value;`
- [x] `fs.readFile(path)` - read file as string
- [x] `fs.writeFile(path, content)` - write string to file
- [x] Array type annotations: `let arr: int[] = [1, 2, 3];`
- [x] `dict` type annotation support

## v0.0.15
- [x] `class{}` wrapper is no longer mandatory (top-level statements allowed)
- [x] Top-level function declarations supported at file scope
- [x] Top-level functions importable from other files
- [x] Recommendation warning when no `class` declaration is present
- [x] Fixed `math.round()` returning FLOAT instead of INT
- [x] Fixed `math.random(max)` exclusive vs inclusive inconsistency
- [x] Fixed `return`/`break`/`continue` crash at top level, in class body, and during `new`
- [x] Fixed scope leak in `execClass` and `execNew` on exception

## v0.0.14
- [x] Fixed scope leak on `return` inside `if`/`while`/`for`/`for-in` blocks
- [x] Fixed `math` module never being registered
- [x] Fixed module function dispatch for `math.sqrt()` etc.
- [x] Fixed `std::stoi`/`std::stod` uncaught `std::out_of_range`
- [x] Fixed UB: `isdigit`/`isalpha`/`isalnum` now receive `unsigned char` cast
- [x] Fixed const variables modifiable via `push()`, `pop()`, `arr[i]=`, `obj.f=`, `++`/`--`
- [x] Fixed `break`/`continue` escaping method call boundaries via `DotCallExpr`
- [x] Fixed duplicate function names across classes (namespaced as `ClassName::funcName`)
- [x] Fixed `math.floor()`/`math.ceil()` returning FLOAT instead of INT
- [x] Fixed duplicate class names silently overwriting
- [x] Added unary minus/plus operator support
- [x] Fixed `static` module registration flag breaking multiple Interpreter instances
- [x] Fixed math functions silently accepting non-numeric arguments

## v0.0.13
- [x] Fixed scope corruption on `continue` in `while`/`for` loops
- [x] Fixed inverted loop safety check (now flags >2s, not <2s)
- [x] Fixed `break`/`continue` escaping function call boundaries
- [x] Fixed `import` crash on missing files (clean error)
- [x] Fixed signed integer overflow UB (now throws runtime error)
- [x] Fixed duplicate function names across classes silently overwriting
- [x] Fixed `new` capturing temporary variables as object fields
- [x] Added `func` as alias for `fn` keyword
- [x] Added `++`/`--` syntax in for-loop update
- [x] Added `math` module to CMake build
- [x] Fixed `math` module function signatures
- [x] Added portable `#include <cctype>`
- [x] Added compiler warnings (`/W4`, `-Wall -Wextra -Wpedantic`)
- [x] Replaced `std::rand()` with `std::mt19937`
- [x] Replaced hardcoded PI with `M_PI`
- [x] Replaced `std::endl` with `\n` in `print`
- [x] Refactored repetitive try/catch into `execBlock` helper
- [x] Improved `extractLine` robustness
- [x] Removed pointless `pushScope`/`popScope` in `execImport`

## v0.1.3
- [x] Fixed `for-in` loops lacking loop safety protection (BUG-5)
- [x] `@unsafe` annotation now applies to `for-in` loops
- [x] `math.log(x)`, `math.log2(x)`, `math.log10(x)` — logarithms
- [x] `math.exp(x)` — exponential
- [x] `math.cbrt(x)` — cube root
- [x] `math.asin(x)`, `math.acos(x)`, `math.atan(x)`, `math.atan2(y, x)` — inverse trig
- [x] `math.degrees(x)`, `math.radians(x)` — angle conversion
- [x] `math.gcd(a, b)` — greatest common divisor
- [x] `math.sign(x)` — sign function
- [x] `math.clamp(v, min, max)` — clamp to range
- [x] `math.factorial(n)` — factorial
- [x] `math.fibonacci(n)` — Fibonacci via fast doubling O(log n)

## v0.1.0
- [x] Fixed `this.field = value` parse error (BUG-1)
- [x] Fixed `this.method()` parse error (BUG-2)
- [x] Fixed method-to-method calls losing object context (BUG-3)
- [x] Fixed `this.field` shadowed by params (BUG-4)
- [x] Fixed `execNew` re-executing side-effect statements (BUG-5)
- [x] Fixed nested imports not being processed (BUG-6)
- [x] Fixed `math.random(min, max)` rejecting `min == max` (BUG-12)
- [x] C++ quality: `noexcept` on simple getters (`toDouble`, `isNumeric`, factory methods, `Lexer::cur`, `Lexer::peek`)
- [x] C++ quality: `const` correctness on `isVarConst` and `isTruthy`
- [x] `graph` module: `scatter`, `line`, `bar`, `hist` SVG plots
- [x] `graph` module: `title`, `xlabel`, `ylabel` plot configuration
- [x] `graph` module: `save(path)` SVG file export, `show()` open in viewer

## Future
- [ ] Self-hosting (rewrite compiler in VDX)
- [ ] AI-specific features
- [ ] Game-specific features
- [ ] `graph` module: multiple series, legends, colors
- [ ] `graph` module: log-scale axes
- [ ] `graph` module: PNG export via third-party lib
- [ ] `graph` module: cross-platform `show()` (macOS/Linux)
