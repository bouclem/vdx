# VDX Internals — Token Types, AST, Value System, Scope, Dispatch

## Token Types

| Token | Pattern | Example |
|-------|---------|---------|
| `STRING` | `"..."` with escapes | `"hello\n"` |
| `INTEGER` | digits, hex `0x…` (v0.1.5+) | `42`, `0`, `0xFF` |
| `FLOAT` | digits `.` digits; also `1.`, `.5`, `1e5`, `1.5e-3` (v0.1.5+) | `3.14`, `5.0` |
| `IDENTIFIER` | alpha/alnum + `_` | `myVar`, `x1` |
| `KW_CLASS` | `class` | |
| `KW_PRINT` | `print` | |
| `KW_LET` | `let` | |
| `KW_CONST` | `const` | |
| `KW_FN` | `fn` or `func` | |
| `KW_RETURN` | `return` | |
| `KW_IF` / `KW_ELIF` / `KW_ELSE` | | |
| `KW_WHILE` | `while` | |
| `KW_FOR` / `KW_IN` | | |
| `KW_NEW` | `new` | |
| `KW_THIS` | `this` | |
| `KW_TRUE` / `KW_FALSE` | | |
| `KW_BREAK` / `KW_CONTINUE` | | |
| `KW_IMPORT` | `import` | |
| `KW_WAIT` | `wait` | |
| `KW_AT_UNSAFE` | `@unsafe` | |
| Operators | `+ - * / % == != < > <= >= ++ -- = . ! && \|\|` (last 3 added v0.1.5) | |
| Punctuation | `{ } ( ) ; , : [ ]` | |

## AST Nodes

### Expressions
- `StringLiteral`, `IntLiteral`, `FloatLiteral`, `BoolLiteral`
- `IdentifierExpr` — variable reference
- `BinaryExpr` — `left op right`
- `ModuloExpr` — `left % right`
- `LogicalExpr` — `&&`, `||` short-circuit (v0.1.5+)
- `NotExpr` — unary `!` (v0.1.5+)
- `IncDecExpr` — `++x`, `x++`, `--x`, `x--`; target is any lvalue expr (v0.1.5+: `arr[i]++`, `obj.n--`)
- `CallExpr` — `name(args)`
- `ThisExpr` — `this.field`
- `ArrayLiteral` — `[1, 2, 3]`
- `DictLiteral` — `{"key": val}`
- `IndexExpr` — `obj[index]`
- `NewExpr` — `new ClassName(args)`
- `DotExpr` — `obj.field`
- `DotCallExpr` — `obj.method(args)`

### Statements
- `LetStmt` — `let name = expr;` (with optional `: type`, incl. `int[]`, `dict`)
- `PrintStmt` — `print(args);`
- `ReturnStmt` — `return expr;`
- `IfStmt` — with `elifs` vector and `elseBody`
- `WhileStmt` — with `isUnsafe` flag
- `ForStmt` — init, condition, update, body, `isUnsafe`
- `ForInStmt` — varName, iterable, body
- `WaitStmt` — `wait(ms);`
- `AssignStmt` — `name = expr;`
- `IndexAssignStmt` — `expr[index] = expr;` (v0.1.5+: any lvalue — `arr[i][j] = v`, `obj.f[i] = v`)
- `DotAssignStmt` — `expr.field = expr;` (v0.1.5+: `this.arr[i] = v` works too)
- `ExprStmt` — standalone expression
- `BreakStmt`, `ContinueStmt`
- `FnDecl` — function declaration
- `ImportStmt` — import statement
- `ClassDecl` — class declaration
- `Program` — root node with declarations vector

## Value System

### Types (Value::Type enum)
| Type | C++ Storage | VDX Literal |
|------|-------------|-------------|
| `STRING` | `std::string strVal` | `"hello"` |
| `INT` | `int intVal` | `42` |
| `FLOAT` | `double floatVal` | `3.14` |
| `BOOL` | `bool boolVal` | `true` |
| `VOID` | (none) | default |
| `ARRAY` | `shared_ptr<vector<Value>> arrVal` (v0.1.5+) | `[1, 2, 3]` |
| `OBJECT` | `shared_ptr<ObjectData> objVal` | `new Class()` |
| `DICT` | `shared_ptr<unordered_map<string,Value>> dictVal` (v0.1.5+) | `{"k": v}` |

Arrays and dicts share storage on copy — `let b = a` aliases `a` (v0.1.5+). This is what makes `obj.field[i] = v`, `push(obj.items, x)`, and `arr[i][j] = v` work.

### ObjectData
```cpp
struct ObjectData {
    std::string className;
    std::unordered_map<std::string, Value> fields;
    std::unordered_map<std::string, const FnDecl*> methods;
};
```

### Truthiness Rules
| Type | Truthy | Falsy |
|------|--------|-------|
| `int` | != 0 | == 0 |
| `float` | != 0.0 | == 0.0 |
| `bool` | `true` | `false` |
| `string` | non-empty | empty `""` |
| `array` | non-empty | empty `[]` |
| `object` | always | never |
| `dict` | non-empty | empty |
| `void` | never | always |

### Type Coercion
- `toDouble()` — int→double for mixed arithmetic
- int + float → float (auto-promotion)
- int / int → int (truncation division)
- String + string → concatenation
- String + non-string → concatenation with `toString()` representation

## Scope System

- Stack of `unordered_map<string, ScopeEntry>` where `ScopeEntry` = `{Value, bool isConst}`
- `pushScope()` / `popScope()` manage the stack
- `lookupVar()` searches from top to bottom
- `declareVar()` adds to current (top) scope
- Block scoping: `if`, `while`, `for`, `for-in` each push a scope
- `ReturnException`, `BreakException`, `ContinueException` all pop scopes before propagating
- Function calls push a param scope; method calls push object-fields scope + param scope

## Function Resolution

1. Namespaced lookup: `currentClassName + "::" + callName` (e.g., `Point::distance`)
2. Plain name lookup: `callName` in `functions` map (top-level + imported fns)
3. Built-in check: `len`, `push`, `pop`, `type`, `input`, `int`, `float`, `str`, `split`, `substr`, `indexOf`, `upper`, `lower`, `trim`, `replace`, `join` (v0.1.5+: user functions resolve BEFORE built-ins — a user `fn len(x)` wins)
4. Module function check: `moduleFunctions` map (e.g., `math.sqrt`, `fs.readFile`)
5. If none match → "Undefined function" error

### currentClassName / currentObject Tracking
- Set during `execClass` body execution
- Set during `execNew` (object construction)
- Set during `DotCallExpr` (method execution)
- Saved and restored on every call (v0.1.5+: `this` no longer leaks into plain functions called from methods)
- `lookupVar` falls through to `currentObject->fields` — bare `x` and `this.x` are the same storage inside a method (v0.1.5+; no more field-scope copies)

## Module Function Dispatch

Module functions (`math.*`, `fs.*`, `graph.*`) are registered in a separate `moduleFunctions` map. They are dispatched via:
- `DotCallExpr` / `DotExpr` — when the object is an `IdentifierExpr` AND `lookupVar` finds no variable with that name; a user variable named `math`/`fs`/`graph` shadows the module (v0.1.5+)
- `callModule()` wrapper appends `at line N` to module errors that lack it, so all module errors get source context (v0.1.5+)

## Import System

- `import "file.vdx";` at top of file
- Resolves relative to source directory
- Imported file is lexed, parsed, and its classes/functions registered
- Classes go into `classDecls` map; methods registered as `ClassName::funcName`
- Top-level functions (outside class) are also registered in `functions` map by plain name
- The entry file is inserted into `importedFiles` at `run()` start (v0.1.5+) — circular imports through main are a clean skip, not a duplicate-definition error
- Each `FnDecl` records its source file so runtime errors inside imports point at the right file (v0.1.5+)
- Top-level statements in imported files are NOT executed — only declarations register
- Circular import protection via `importedFiles` set
- Transitive imports NOT processed — each file must be imported directly
- Imported class methods are namespaced, not global — call them via `new ClassName()` + dot-call

## Safety Guards (v0.1.5+)

- Call depth ≤ 500 — deeper throws "Maximum call depth exceeded" instead of segfaulting (Windows link stack raised to 16 MB so the guard fires first)
- Loop iterations ≤ 1,000,000 per loop — catches `while(true) {}` that the old per-iteration timer missed
- Per-iteration wall time > 2000 ms still errors; `wait()`/`input()` blocking time is excluded
- `@unsafe` before `while`/`for`/`for-in` disables both checks
- Parser expression depth is bounded — deeply nested `((((...))))` errors instead of overflowing the C++ stack

## Error Handling

### Error Types
- **Lexer errors**: unterminated string, unexpected character, unknown annotation
- **Parser errors**: unexpected token, expected expression, `break`/`continue` outside a loop, `fn` nested inside a block (v0.1.5+ — was silently dropped), expression depth exceeded
- **Runtime errors**: undefined variable/function, type mismatch, index out of bounds, division by zero (incl. `INT_MIN / -1`, v0.1.5+), const violation, loop safety (timeout or 1M-iteration cap), call depth exceeded, break/continue outside loop (graceful warning in v0.0.15+ for top-level/class-body/new contexts)
- **Out-of-range literals**: integer/float literals exceeding type limits

### Error Output Format
```
  error: [VDX] Error message at line N
   --> filename.vdx:N
    |
  N-1 |  previous line
  N   |  error line  <--
  N+1 |  next line
    |
```
