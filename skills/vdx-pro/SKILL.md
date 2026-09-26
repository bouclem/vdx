---
name: vdx-pro
description: VDX coding standards, best practices, and production-quality code guidance. Use when writing, reviewing, or refactoring VDX code. Covers naming, structure, safety, patterns, and common pitfalls.
---

# VDX Pro — Coding Standards & Best Practices

## When to Use

- Writing new VDX programs
- Reviewing or refactoring VDX code
- Setting up project structure and naming conventions
- Solving VDX-specific syntax issues (dict dot-access, module shadowing, etc.)

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
- `for-in`: `for (item in arr) { ... }` — arrays (elements), strings (characters, v0.1.5+), dicts (keys in sorted order, v0.1.5+)
- `break` — exit loop (parse error outside a loop, v0.1.5+)
- `continue` — skip to next iteration (parse error outside a loop, v0.1.5+)
- `wait(ms)` — pause execution; accepts int and float (v0.1.5+): `wait(0.5)`

### Loop Safety
- An iteration taking >2000ms triggers an error by default (all loop types: `while`, `for`, `for-in`)
- A loop exceeding 1,000,000 iterations errors too — catches `while(true) {}` (v0.1.5+)
- `wait()`/`input()` blocking time is NOT counted against the 2s/iteration budget (v0.1.5+)
- Use `@unsafe` before `while`, `for`, or `for-in` to disable both checks: `@unsafe while (true) { ... }`

### Operators
- Arithmetic: `+`, `-`, `*`, `/`, `%` (unary `-x`/`+x` supported)
- Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=` (v0.1.5+: works on strings too — lexicographic)
- Logical (v0.1.5+): `!`, `&&`, `||` — short-circuit; operands use truthiness rules
- Increment/Decrement: `++x`, `x++`, `--x`, `x--` — v0.1.5+: works on any lvalue (`arr[i]++`, `obj.n--`, `this.count++`)
- String concatenation: `+` (string + string only; string + int/float variable throws — use comma-separated `print` args or `str(n)`)
- Assignment targets (v0.1.5+): `name = v`, `arr[i] = v`, `arr[i][j] = v`, `obj.f = v`, `obj.f[i] = v`, `this.arr[i] = v`, `s[i] = "c"` (string char assign), `d["newKey"] = v` (creates the key)

### Types
- `int` — integer literals: `42`, `0`, `999`; hex `0xFF` (v0.1.5+)
- `float` — decimal literals: `3.14`, `5.0`; also `1.`, `.5`, `1e5`, `1.5e-3` (v0.1.5+)
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
- `push(arr, value)` — append; v0.1.5+: first arg can be any lvalue (`push(obj.items, x)`, `push(this.arr, x)`)
- `pop(arr)` — remove and return last element (same lvalue support)
- String indexing: `str[0]` returns single-character string; `str[i] = "c"` assigns (v0.1.5+)
- Empty array `[]` is falsy; non-empty is truthy
- **Reference semantics (v0.1.5+)**: `let b = arr` shares storage — `push(b, x)` also changes `arr`

### Dictionaries
- Literal: `let d = {"name": "Alice", "age": 30};`
- Access: `d["name"]`
- Assign: `d["city"] = "Paris";`
- `len(d)` — number of keys
- Keys must be strings
- `d["newKey"] = v` creates the key if missing (v0.1.5+)
- `for (k in d)` iterates keys in sorted order (v0.1.5+)
- No dot access — use `d["k"]`, not `d.k`

### Built-in Functions
- `print(args...)` — print space-separated
- `len(x)` — array/string/dict/object field count
- `push(arr, val)` — append to array (lvalue arg)
- `pop(arr)` — remove and return last (lvalue arg)
- `type(x)` — return type name string: "int", "float", "string", "bool", "array", "object", "dict", "void"
- `input(prompt?)` — read line from stdin, optional prompt string
- Conversions (v0.1.5+): `int(v)`, `float(v)`, `str(v)` — e.g., `int("42")`, `str(3.14)`
- String functions (v0.1.5+): `split(s, sep)`, `substr(s, i, len)`, `indexOf(s, sub)`, `upper(s)`, `lower(s)`, `trim(s)`, `replace(s, old, new)`, `join(arr, sep)`
- User-defined `fn` may reuse built-in names — the user function wins (v0.1.5+)

### Modules
- `math.sqrt(x)`, `math.pow(base, exp)`, `math.abs(x)`
- `math.sin(x)`, `math.cos(x)`, `math.tan(x)` (radians)
- `math.floor(x)`, `math.ceil(x)`, `math.round(x)`
- `math.min(a, b, ...)`, `math.max(a, b, ...)`
- `math.random()`, `math.random(max)`, `math.random(min, max)`
- `math.pi`, `math.e`, `math.tau` — constants (many more in v0.1.3/v0.1.4: log, exp, gcd, fibonacci, sort, mean, ...)
- `fs.readFile(path)` — read file as string
- `fs.writeFile(path, content)` — write string to file
- `fs.append(path, content)` — append to file (v0.1.5+)
- `fs.exists(path)` — check path exists (v0.1.5+)
- `fs.listDir(path)` — list directory entries (v0.1.5+)
- `fs.setRoot(dir)` — opt-in sandbox: all fs paths must stay inside `dir` once set (v0.1.5+; unrestricted by default)
- `graph.scatter/line/bar/hist/area`, `graph.title/xlabel/ylabel`, `graph.save/show`, `graph.color/grid/legend` — SVG plotting
- **Module shadowing**: a variable named `math`, `fs`, or `graph` wins over the module (v0.1.5+) — `math.x` then looks up field `x` on YOUR variable. Name things `mu`, `filesys`, etc.

### Imports
- `import "filename.vdx";` — imports classes and functions from another file
- Imported top-level functions are callable directly: `import "utils.vdx"; let x = add(1, 2);`
- Imported class methods are namespaced — call via an object: `let u = new Utils(); u.add(1, 2);`
- Circular imports are detected and prevented (including back through the entry file, v0.1.5+)
- Transitive imports are NOT processed (A imports B, B imports C → C not available to A)
- Top-level statements in imported files do NOT execute — declarations only

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
- Use `@unsafe` only when legitimate slow/long loops are needed
- Loops are capped at 1M iterations and 2s/iteration — long-running loops need `@unsafe`
- Keep recursion under 500 calls — deeper recursion throws
- `fs.setRoot()` before file ops keeps scripts inside a sandbox directory

## Common Pitfalls

- **Arrays/dicts are shared on assign (v0.1.5+)**: `let b = a; push(b, x)` changes `a` too. Copy elements manually if you need an independent list
- **Module names get shadowed (v0.1.5+)**: `let math = new Foo();` makes `math.sqrt` fail — your variable wins. Avoid `math`/`fs`/`graph` as variable names
- **No dict dot-access**: `d.key` is for objects/modules — use `d["key"]`
- **push/pop need an lvalue**: `push([1,2], 1)` fails (literal isn't assignable); `push(obj.items, 1)` works
- **Integer division truncates**: `7 / 2 = 3`. Use `7.0 / 2.0` or `float(x) / y` for float
- **Imported class methods aren't global**: `import "utils.vdx"` with `class Utils { fn add... }` requires `new Utils()` + `u.add(...)`, not `add(...)`
- **Class is optional but recommended**: No `class{}` needed for simple scripts; a stderr tip is printed if no class is present
- **String + int variable throws**: Use `print("count:", x)` or `print("count: " + str(x))` not `print("count: " + x)`
- **`break`/`continue` outside loops = parse error (v0.1.5+)**; `fn` nested inside `if`/loops/`fn` = parse error — declare functions at top level or in class bodies
- **Deep nesting caps**: recursion >500 calls or absurd expression depth throws — flatten deep recursion
- **Transitive imports don't work**: Import each file directly

## References

- See `references/patterns.md` for code patterns (singleton, data class, collection iteration, conditional without logical operators, class-free scripts)
- See `examples/examples.vdx` for a complete working demo of all features
- See `examples/utils.vdx` for import pattern example
