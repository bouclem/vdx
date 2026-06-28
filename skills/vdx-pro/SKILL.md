---
name: vdx-pro
description: VDX coding standards, best practices, and production-quality code guidance. Use when writing, reviewing, or refactoring VDX code. Covers naming, structure, safety, patterns, and common pitfalls.
---

# VDX Pro — Coding Standards & Best Practices

## When to Use

- Writing new VDX programs
- Reviewing or refactoring VDX code
- Setting up project structure and naming conventions
- Solving VDX-specific syntax issues (no unary minus, no logical operators, etc.)

## Language Overview

VDX is a class-based, interpreted programming language. Starting with v0.0.15, the `class{}` wrapper is optional — top-level statements and functions can be written directly at file scope. Classes are still recommended for larger programs and are required for object instantiation. Entry point is any file with top-level statements (non-function statements execute top-to-bottom).

## File Structure

- File extension: `.vdx`
- Comments: `// line comments` only
- Imports must be first: `import "other.vdx";`
- Class declarations follow imports (optional — file can have zero classes)
- One primary class per file is conventional for larger programs

## Syntax Rules

### Class Declaration
```vdx
class ClassName {
    // Fields (let declarations)
    let x: int = 0;
    let name: string = "default";

    // Constants
    const MAX: int = 100;

    // Methods (fn or func)
    fn methodName(param1, param2) {
        // method body
        return value;
    }

    // Top-level statements execute at load time
    print("initializing...");
}
```

### Variables
- `let` — mutable variable: `let x = 5;`
- `const` — immutable: `const PI: float = 3.14;`
- Type annotations (optional): `let x: int = 5;`, `let s: string = "hi";`, `let f: float = 3.14;`, `let b: bool = true;`
- Array type: `let nums: int[] = [1, 2, 3];`
- Dict type: `let user: dict = {"name": "Bob"};`
- Reassignment: `x = newValue;`
- Const cannot be modified via any path (direct assign, push, pop, index assign, dot assign, ++/--)

### Functions
- Declare with `fn` or `func` (aliases): `fn add(a, b) { return a + b; }`
- No explicit parameter types
- `return` exits function with optional value
- Functions called by name within same class: `add(3, 5)`
- Top-level functions (outside class) can be called directly: `add(3, 5)`
- Cross-class calls require object instantiation and dot-call: `obj.method(args)`
- Top-level functions are importable from other files (v0.0.15+)

### Control Flow
- `if` / `elif` / `else`: standard syntax with `{ }` blocks
- `while (condition) { ... }`
- C-style `for`: `for (let i = 0; i < n; i = i + 1) { ... }` or `for (let i = 0; i < n; i++) { ... }`
- `for-in`: `for (item in arr) { ... }` (arrays only, not dicts or strings)
- `break` — exit loop
- `continue` — skip to next iteration
- `wait(ms)` — pause execution (integer milliseconds)

### Loop Safety
- Iterations taking >2000ms trigger an error by default
- Use `@unsafe` before `while` or `for` to bypass: `@unsafe while (true) { ... }`
- `for-in` loops do NOT have safety checks — be cautious with large arrays

### Operators
- Arithmetic: `+`, `-`, `*`, `/`, `%`
- Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
- Increment/Decrement: `++x`, `x++`, `--x`, `x--`
- String concatenation: `+` (string + string only; string + int/float variable throws — use comma-separated `print` args instead)
- NO unary minus — use `0 - x` instead of `-x`
- NO logical operators (`&&`, `||`, `!`) — use nested if or arithmetic tricks

### Types
- `int` — integer literals: `42`, `0`, `999`
- `float` — decimal literals: `3.14`, `5.0`
- `string` — double-quoted: `"hello"`, escapes: `\n`, `\t`, `\\`, `\"`
- `bool` — `true` / `false`
- `array` — `[1, 2, 3]`, `["a", "b"]`, mixed types allowed
- `dict` — `{"key": value, ...}` (keys must be string literals)
- `object` — instance of a class via `new`
- `void` — no value (default return)

### Type Promotion
- int + float = float (auto-promotion)
- Division: int / int = int (truncates), float involved = float result
- Modulo: int % int = int, any float = float result

### Objects
- Create: `let obj = new ClassName();`
- Field access: `obj.fieldName`
- Field assignment: `obj.fieldName = value;`
- Method call: `obj.methodName(args)`
- Methods can call sibling methods by bare name: `helper()` (resolves within same class)
- Methods can modify object fields via direct assignment
- `this` keyword accesses class-scope variables
- Objects print as `<ClassName object>`

### Arrays
- Literal: `let arr = [1, 2, 3];`
- Index: `arr[0]` (0-based)
- Index assign: `arr[0] = 99;`
- `len(arr)` — length
- `push(arr, value)` — append (first arg must be variable name, not expression)
- `pop(arr)` — remove and return last element (first arg must be variable name)
- String indexing: `str[0]` returns single-character string
- Empty array `[]` is falsy; non-empty is truthy

### Dictionaries
- Literal: `let d = {"name": "Alice", "age": 30};`
- Access: `d["name"]`
- Assign: `d["city"] = "Paris";`
- `len(d)` — number of keys
- Keys must be strings
- Empty dict `{}` is NOT supported — use `{"": 0}` as workaround or declare with at least one entry
- Iteration order is non-deterministic (backing is unordered_map)

### Built-in Functions
- `print(args...)` — print space-separated
- `len(x)` — array/string/dict/object field count
- `push(arr, val)` — append to array
- `pop(arr)` — remove and return last
- `type(x)` — return type name string: "int", "float", "string", "bool", "array", "object", "dict", "void"
- `input(prompt?)` — read line from stdin, optional prompt string

### Modules
- `math.sqrt(x)`, `math.pow(base, exp)`, `math.abs(x)`
- `math.sin(x)`, `math.cos(x)`, `math.tan(x)` (radians)
- `math.floor(x)`, `math.ceil(x)`, `math.round(x)`
- `math.min(a, b, ...)`, `math.max(a, b, ...)`
- `math.random()`, `math.random(max)`, `math.random(min, max)`
- `math.pi` — constant
- `fs.readFile(path)` — read file as string
- `fs.writeFile(path, content)` — write string to file

### Imports
- `import "filename.vdx";` — imports classes and functions from another file
- Imported functions are namespaced as `ClassName::funcName` internally
- Circular imports are detected and prevented
- Transitive imports are NOT processed (A imports B, B imports C → C not available to A)

## Coding Standards

### Naming
- Classes: `PascalCase` — `MyClass`, `Point`, `Main`
- Functions/methods: `camelCase` — `getName`, `calculateArea`
- Variables: `camelCase` — `firstName`, `totalCount`
- Constants: `UPPER_SNAKE` — `MAX_SIZE`, `PI`, `DEFAULT_TIMEOUT`
- Booleans: prefix `is`/`has`/`can` — `isActive`, `hasData`

### Structure
- One class per file when possible
- Fields first, then methods, then top-level statements
- Keep methods short — one purpose per method
- Avoid deeply nested blocks (>3 levels) — extract helper methods

### Safety
- Use `const` for values that should never change
- Use type annotations for clarity in public APIs
- Use `@unsafe` only when legitimate slow operations are needed
- Avoid infinite loops without `@unsafe` — killed after 2s

## Common Pitfalls

- **No unary minus**: `let x = -5;` → syntax error. Use `let x = 0 - 5;`
- **No empty dict**: `{}` → parse error. Use `{"": 0}`
- **for-in only works on arrays**: Not dicts or strings
- **push/pop first arg must be a variable**: `push(arr, 1)` works, `push([1,2], 1)` does not
- **Integer division truncates**: `7 / 2 = 3`. Use `7.0 / 2.0` for float
- **No logical operators**: No `&&`, `||`, `!`. Use nested if
- **Class is optional but recommended**: No `class{}` needed for simple scripts; a stderr tip is printed if no class is present
- **String + int variable throws**: Use `print("count:", x)` not `print("count: " + x)`
- **Dict key order is non-deterministic**: Don't rely on insertion order
- **Transitive imports don't work**: Import each file directly

## References

- See `references/patterns.md` for code patterns (singleton, data class, collection iteration, conditional without logical operators, class-free scripts)
- See `examples/examples.vdx` for a complete working demo of all features
- See `examples/utils.vdx` for import pattern example
