# VDX Changelog

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
