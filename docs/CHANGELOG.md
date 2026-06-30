# VDX Changelog

## v0.1.2 — 2026-06-30
- Fixed bare method calls (called by name from within another method) losing object field mutations when `break` or `continue` is thrown — fields are now synced before the error is raised
- Fixed `graph.bar()` drawing bars from `vMin` instead of the zero line for mixed positive/negative values — bars now correctly start from zero
- Fixed `Value::toString` not escaping inner quotes in strings inside arrays and dicts — now properly escapes `\`, `"`, `\n`, `\t`, `\r`
- Fixed `escXml` in graph module not escaping control characters (`\n`, `\r`, `\t`) — now escapes all control chars as numeric XML entities to prevent malformed SVG
- Fixed graph module `PlotState` being shared across threads — changed to `thread_local` to isolate state per thread
- Fixed lexer not handling `\r` and `\0` escape sequences in string literals — now correctly produces carriage return and null character
- Fixed `math.floor()`, `math.ceil()`, `math.round()` causing undefined behavior on very large float values — now checks `INT_MAX`/`INT_MIN` bounds before casting
- Fixed `graph.show()` using `tmpnam_s` with a TOCTOU race condition — now uses `std::filesystem::temp_directory_path()` with a random filename

## v0.1.1 — 2026-06-30
- Fixed `for-in` iterating over a copy of the array instead of the live array — modifications (push/pop/index assign) during iteration are now reflected
- Fixed `DotCallExpr` method calls losing object field mutations when `break` or `continue` is thrown inside a method — fields are now synced before the error is raised
- Fixed `graph.bar()` rendering incorrectly with all-negative values — now computes proper vMin from data and clamps to 0
- Fixed integer overflow in `++`/`--` operators not being detected — now throws runtime error matching `+`/`-`/`*` overflow checks
- Fixed empty dict literal `{}` causing a parse error — now correctly produces an empty dictionary

## v0.1.0 — 2026-06-29
- **`this` system overhaul** — `this` now returns the actual object instead of doing scope lookup, fixing param/field shadowing bugs
- Fixed `this.field = value` parse error — `this` field assignment now works inside methods
- Fixed `this.method()` parse error — `this` method calls now work inside methods
- Fixed method-to-method calls losing object context — calling a bare method name from within another method now properly pushes object fields as scope and syncs them back
- Fixed `execNew` re-executing side-effect statements (e.g. `print`) on every `new` — only `let` field initializers are executed now
- Fixed `execClass` executing non-function statements at class declaration time — now only registers methods
- Fixed nested imports not being processed — imported files' own `import` statements are now resolved recursively with correct relative path resolution
- Fixed `math.random(min, max)` rejecting `min == max` — now returns `min` instead of erroring
- **C++ quality improvements**: `noexcept` on simple getters (`toDouble`, `isNumeric`, `makeInt`, `makeFloat`, `makeBool`, `makeVoid`, `Lexer::cur`, `Lexer::peek`), `const` correctness on `isVarConst` and `isTruthy`
- **`graph` module** — new plotting and data visualization module (SVG vector graphics output):
  - `graph.scatter(xs, ys)` — scatter plot from two numeric arrays
  - `graph.line(xs, ys)` — line chart connecting points
  - `graph.bar(labels, values)` — vertical bar chart
  - `graph.hist(data, [bins])` — histogram with configurable bin count
  - `graph.title(text)`, `graph.xlabel(text)`, `graph.ylabel(text)` — plot configuration
  - `graph.save(path)` — save current plot as SVG file
  - `graph.show()` — open current plot in default viewer

## v0.0.15 — 2026-06-28
- `class{}` wrapper is no longer mandatory — top-level statements (print, let, fn, if, while, for, etc.) can now be written directly without a class wrapper
- Top-level function declarations are now supported: `fn add(a, b) { ... }` at file scope
- Top-level functions are importable: imported files can export plain functions (not just class methods)
- Added recommendation warning: when a VDX file has no `class` declaration, a tip is printed to stderr suggesting `class{}` for better organization
- `class{}` is now recommended but optional — existing code with classes continues to work unchanged
- Fixed `math.round()` returning FLOAT instead of INT for float inputs — now returns INT like `math.floor()` and `math.ceil()`
- Fixed `math.random(max)` being exclusive (0 to max-1) — now inclusive (0 to max) for consistency with `math.random(min, max)`
- Fixed `return`/`break`/`continue` at top level (outside function/loop) causing `std::terminate()` crash — now handled gracefully
- Fixed `return`/`break`/`continue` in class body (outside functions) causing `std::terminate()` crash — now handled gracefully
- Fixed `return`/`break`/`continue` during object construction (`new`) causing `std::terminate()` crash — now handled gracefully
- Fixed scope leak in `execClass` on exception — scope is now properly popped on all exit paths
- Fixed scope leak in `execNew` on exception — scope is now properly popped on all exit paths

## v0.0.14 — 2026-06-28
- Fixed scope leak on `return` inside `if` blocks, `while`, `for`, and `for-in` loops — `ReturnException` now pops scope before propagating
- Fixed `math` module never being registered — added missing `registerMath` definition and call at startup
- Fixed module function dispatch for `math.sqrt()` etc. — `DotCallExpr` and `DotExpr` now check `moduleFunctions` before object method/field lookup
- Module constants like `math.pi` now work via `DotExpr` module function lookup with 0 args
- Fixed `std::stoi`/`std::stod` throwing uncaught `std::out_of_range` on out-of-range integer/float literals — now gives clean error with line number
- Fixed undefined behavior: `isdigit`/`isalpha`/`isalnum` in lexer now receive `unsigned char` cast to avoid UB on non-ASCII input
- Fixed const variables modifiable via `push()`, `pop()`, `arr[i]=`, `obj.f=`, and `++`/`--` — all mutation paths now check `isVarConst`
- Fixed `break`/`continue` escaping method call boundaries via `DotCallExpr` — now throws runtime error matching `execCall` behavior
- Fixed duplicate function names across classes causing collision — methods are now namespaced as `ClassName::funcName` in the internal function map, allowing same-named methods in different classes
- Fixed `math.floor()` and `math.ceil()` returning FLOAT instead of INT — now returns INT for whole-number results
- Fixed duplicate class names silently overwriting each other — now throws error on duplicate class declarations (both main file and imports)
- Added unary minus/plus operator support: `let x = -5;`, `let y = -x;`, `5 - -3` now parse correctly
- Fixed `static` module registration flag breaking multiple Interpreter instances — moved to per-instance member variable
- Fixed math functions silently accepting non-numeric arguments (string, bool, array, etc.) — now throws type error

## v0.0.13 — 2026-06-27
- Fixed `continue` in `while` and `for` loops corrupting the scope stack (double popScope)
- Fixed loop safety check logic: now correctly flags iterations taking >2s instead of <2s
- Fixed `break`/`continue` escaping function call boundaries into outer loops — now throws runtime error
- Fixed `import` crashing with unhandled exception when file doesn't exist (now gives clean error)
- Fixed signed integer overflow causing undefined behavior in `+`, `-`, `*` — now throws runtime error
- Fixed duplicate function names across classes silently overwriting each other — now throws error
- Fixed `new` capturing temporary variables (e.g. loop counters) as object fields — only captures `let` declarations
- Added `func` as alias for `fn` keyword: `func add(a, b) { ... }`
- Added `++`/`--` syntax in for-loop update: `for (let i = 0; i < 10; i++) { ... }`
- Added `math` module to CMake build (was missing — dead code)
- Fixed `math` module function signatures to match interpreter's `ModuleFunc` type
- Added `#include <cctype>` for portable `isdigit`/`isalpha` usage in lexer and main
- Added compiler warnings (`/W4` on MSVC, `-Wall -Wextra -Wpedantic` on GCC/Clang)
- Replaced `std::rand()` with `std::mt19937` for better random number quality
- Replaced hardcoded PI with `M_PI` from `<cmath>`
- Replaced `std::endl` with `\n` in `print` to avoid unnecessary flush
- Refactored repetitive try/catch in `if`/`elif`/`else` into `execBlock` helper
- Improved `extractLine` robustness: searches for "at line " instead of just "line "
- Removed pointless `pushScope`/`popScope` in `execImport`

## v0.0.12 — 2026-06-15
- Added dictionary/map type: `let user = {"name": "Alice", "age": 30};`
- Dictionary access: `user["name"]` returns `"Alice"`
- Dictionary assignment: `user["city"] = "Paris";`
- Dictionary length: `len(user)` returns number of key-value pairs
- Added `fs` module with file I/O:
  - `fs.readFile(path)` - reads file contents as string
  - `fs.writeFile(path, content)` - writes string to file
- Added array type annotations: `let nums: int[] = [1, 2, 3];`
- Added `dict` type annotation: `let user: dict = {"name": "Bob"};`

## v0.0.11 — 2026-06-15
- Added `import` statement: import other VDX files with `import "filename.vdx";`
- Imported files provide access to their classes and functions
- Circular import protection prevents infinite loops
- Added `type(value)` built-in: returns type name as string ("int", "float", "string", "bool", "array", "object", "void")
- Added `input()` built-in: read user input from stdin
- Added `input(prompt)` variant: print prompt then read input
- Added `pop(arr)` built-in: remove and return last element from array
- Extended `len(obj)`: now works with objects, returning number of fields
- `len()` now supports arrays, strings, and objects

## v0.0.10 — 2026-06-08
- Added modulo operator `%`: calculate remainder (e.g., `10 % 3` = 1)
- Added increment `++` and decrement `--` operators
- Prefix form: `++x` and `--x` (returns value after operation)
- Postfix form: `x++` and `x--` (returns value before operation)
- MSI installer now asks to add vdx to system PATH (optional checkbox)

## v0.0.9 — 2026-06-08
- Added `break` statement: exit loops early with `break;`
- Added `continue` statement: skip to next loop iteration with `continue;`
- Added `const` keyword: declare constants that cannot be reassigned
- Constants support type annotations: `const PI: float = 3.14;`
- Added `math` module with functions:
  - `math.sqrt(x)` — square root
  - `math.pow(base, exp)` — power
  - `math.abs(x)` — absolute value
  - `math.sin(x)`, `math.cos(x)`, `math.tan(x)` — trigonometry (radians)
  - `math.floor(x)`, `math.ceil(x)`, `math.round(x)` — rounding
  - `math.min(a, b, ...)` — minimum value
  - `math.max(a, b, ...)` — maximum value
  - `math.random()` — random float 0-1
  - `math.random(max)` — random int 0 to max-1
  - `math.random(min, max)` — random int min to max
  - `math.pi` — constant PI value
- Created `vdx/src/modules/` directory for built-in modules
- Interpreter tracks const variables to prevent reassignment
- Loop control statements work with `while`, `for`, and `for-in` loops
- Break/continue exceptions properly propagate through function calls

## v0.0.8 — 2026-03-23
- Added `float` type: float literals with decimal point (e.g., `3.14`, `5.0`)
- Added `true` / `false` boolean literals
- Added optional type annotations on `let`: `let x: int = 5;`, `let pi: float = 3.14;`, `let name: string = "hi";`, `let ok: bool = true;`
- Runtime type checking: annotated variables are validated at assignment
- Mixed int/float arithmetic: operations auto-promote to float when either operand is float
- Added `new` keyword for object instantiation: `let obj = new ClassName();`
- Dot field access: `obj.field` reads a field from an object
- Dot field assignment: `obj.field = value;` sets a field on an object
- Dot method calls: `obj.method(args)` calls a method with access to object fields
- Objects print as `<ClassName object>`
- Added C-style `for` loop: `for (let i = 0; i < n; i = i + 1) { ... }`
- Added `for-in` loop over arrays: `for (item in arr) { ... }`
- `@unsafe` now also works with `for` loops
- Float truthiness: `0.0` is falsy, non-zero is truthy
- Object truthiness: objects are truthy

## v0.0.7 — 2026-03-22
- Added arrays / lists: `let arr = [1, 2, 3];`
- Array index access: `arr[0]`
- Array index assignment: `arr[0] = 5;`
- String index access: `str[0]` returns single character
- Built-in `len()`: returns length of arrays and strings
- Built-in `push(arr, value)`: appends a value to an array
- Array printing: `print(arr)` outputs `[1, 2, 3]`
- Array truthiness: non-empty arrays are truthy, empty arrays are falsy
- Improved error reporting: errors now display file name, line number, and surrounding source lines with an arrow pointing to the error line

## v0.0.6 — 2026-03-22
- Added loop safety protection: `while` loops that iterate faster than 2 seconds are blocked by default
- Added `@unsafe` annotation: place before `while` to bypass loop speed protection
- Added SVG icon (`assets/icon.svg`)
- Added Windows MSI installer support via CMake CPack + WiX
- Installer adds `vdx` to system PATH (run `vdx file.vdx` from any terminal)
- Added `.vdx` file association in installer
- Added LICENSE.txt (MIT)
- Website v0.1.0: main page, download page (with versioned .msi downloads), documentation pages

## v0.0.5 — 2026-03-22
- Added `while` loop: `while (condition) { body }`
- Added block scoping: variables declared inside `{ }` blocks are local to that block
- Added `wait(ms)` statement to pause execution for a given number of milliseconds
- Added variable reassignment: `name = expr;` (required for mutable loop counters)

## v0.0.4 — 2026-03-22
- Added `this` keyword for accessing class-scope variables (`this.field`)
- Added `if` / `elif` / `else` control flow
- Inline expression evaluation in `print()` (e.g., `print(1 + 1)` outputs `2`)
- Truthiness rules: int != 0, non-empty string, bool value, void = false

## v0.0.3 — 2026-03-22
- Added `fn` keyword for function declarations with parameters
- Added `return` statement for function return values
- Function calls as expressions (usable in `let`, `print`, other calls)
- Expression statements (standalone function calls)
- Operators: `+`, `-`, `*`, `/` (arithmetic)
- Operators: `==`, `!=`, `<`, `>`, `<=`, `>=` (comparison)
- String concatenation with `+`
- Operator precedence: `* /` before `+ -` before comparisons before equality
- Parenthesized expressions for grouping
- Typed value system (string, int, bool, void)
- Division by zero runtime error
- Two-pass class execution (register functions first, then run statements)

## v0.0.2 — 2026-03-22
- Added `let` keyword for variable declarations
- Variables support string and integer values
- Variables can be used in `print()` arguments
- Added expression system (string literals, integer literals, identifiers)
- Updated example to demonstrate variable usage

## v0.0.1 — 2026-03-22
- Initial release
- Lexer with support for strings, integers, identifiers, keywords, symbols
- Parser for `class` declarations and `print()` statements
- Tree-walking interpreter
- `class` keyword: declare named classes with a body
- `print()`: output string literals to stdout
- Line comments (`//`)
- Example: `examples/hello.vdx`
