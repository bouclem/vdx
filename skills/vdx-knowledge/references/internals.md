# VDX Internals — Token Types, AST, Value System, Scope, Dispatch

## Token Types

| Token | Pattern | Example |
|-------|---------|---------|
| `STRING` | `"..."` with escapes | `"hello\n"` |
| `INTEGER` | digits | `42`, `0` |
| `FLOAT` | digits `.` digits | `3.14`, `5.0` |
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
| Operators | `+ - * / % == != < > <= >= ++ -- = .` | |
| Punctuation | `{ } ( ) ; , : [ ]` | |

## AST Nodes

### Expressions
- `StringLiteral`, `IntLiteral`, `FloatLiteral`, `BoolLiteral`
- `IdentifierExpr` — variable reference
- `BinaryExpr` — `left op right`
- `ModuloExpr` — `left % right`
- `IncDecExpr` — `++x`, `x++`, `--x`, `x--`
- `CallExpr` — `name(args)`
- `ThisExpr` — `this.field`
- `ArrayLiteral` — `[1, 2, 3]`
- `DictLiteral` — `{"key": val}`
- `IndexExpr` — `obj[index]`
- `NewExpr` — `new ClassName(args)`
- `DotExpr` — `obj.field`
- `DotCallExpr` — `obj.method(args)`

### Statements
- `LetStmt` — `let name = expr;` (with optional `: type`)
- `PrintStmt` — `print(args);`
- `ReturnStmt` — `return expr;`
- `IfStmt` — with `elifs` vector and `elseBody`
- `WhileStmt` — with `isUnsafe` flag
- `ForStmt` — init, condition, update, body, `isUnsafe`
- `ForInStmt` — varName, iterable, body
- `WaitStmt` — `wait(ms);`
- `AssignStmt` — `name = expr;`
- `IndexAssignStmt` — `name[index] = expr;`
- `DotAssignStmt` — `obj.field = expr;`
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
| `ARRAY` | `std::vector<Value> arrVal` | `[1, 2, 3]` |
| `OBJECT` | `shared_ptr<ObjectData> objVal` | `new Class()` |
| `DICT` | `unordered_map<string,Value> dictVal` | `{"k": v}` |

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
| `dict` | non-empty | empty (but `{}` not parseable) |
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

1. Built-in check: `len`, `push`, `pop`, `type`, `input`
2. Module function check: `moduleFunctions` map (e.g., `math.sqrt`, `fs.readFile`)
3. Namespaced lookup: `currentClassName + "::" + callName` (e.g., `Point::distance`)
4. Plain name fallback: `callName` in `functions` map
5. If none match → "Undefined function" error

### currentClassName Tracking
- Set during `execClass` body execution
- Set during `execNew` (object construction)
- Set during `DotCallExpr` (method execution)
- Restored after each completes

## Module Function Dispatch

Module functions (`math.*`, `fs.*`) are registered in a separate `moduleFunctions` map. They are dispatched via:
- `DotCallExpr` — when object is `IdentifierExpr`, constructs `module.func` name and checks `moduleFunctions` before object method lookup
- `DotExpr` — when object is `IdentifierExpr`, checks `moduleFunctions` for constants like `math.pi`
- `CallExpr` — checks `call->name` directly (for non-dotted built-in names)

## Import System

- `import "file.vdx";` at top of file
- Resolves relative to source directory
- Imported file is lexed, parsed, and its classes/functions registered
- Classes go into `classDecls` map
- Functions go into `functions` map as `ClassName::funcName`
- Circular import protection via `importedFiles` set
- Transitive imports NOT processed — each file must be imported directly

## Error Handling

### Error Types
- **Lexer errors**: unterminated string, unexpected character, unknown annotation
- **Parser errors**: unexpected token, expected expression, expected `class`
- **Runtime errors**: undefined variable/function, type mismatch, index out of bounds, division by zero, const violation, loop safety timeout, break/continue outside loop
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
