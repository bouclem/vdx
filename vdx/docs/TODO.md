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

## v0.0.6 (current)
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

## v0.0.8 (current)
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

## Future
- [ ] Self-hosting (rewrite compiler in VDX)
- [ ] AI-specific features
- [ ] Game-specific features
