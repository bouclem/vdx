---
name: vdx-knowledge
description: Complete VDX language reference, runtime behavior, internals, and troubleshooting. Use when understanding VDX architecture, debugging interpreter behavior, or needing deep language knowledge.
---

# VDX Knowledge — Language Reference & Internals

## When to Use

- Understanding VDX architecture and interpreter pipeline
- Debugging VDX runtime behavior or error messages
- Needing deep knowledge of scope, function resolution, or module dispatch
- Checking known limitations and edge cases

## What is VDX?

VDX is a class-based interpreted programming language built in C++17. It runs via a tree-walking interpreter (lexer → parser → AST → interpreter). Designed for speed with future focus on AI and game development.

- **Website**: [voidwarelang.xyz](https://voidwarelang.xyz)
- **Repository**: Voidware/vdx
- **File extension**: `.vdx`
- **Current version**: 0.0.14

## Architecture

### Pipeline
1. **Lexer** (`lexer.cpp`) — Tokenizes source into tokens with line/column tracking
2. **Parser** (`parser.cpp`) — Recursive descent parser, builds AST with `shared_ptr` nodes
3. **Interpreter** (`interpreter.cpp`) — Tree-walking evaluator with stack-based scopes

### Key Design Decisions
- All code must be inside `class` bodies — no bare top-level statements
- `shared_ptr` for AST node ownership
- Stack of `unordered_map` scopes for variable lookup
- Exceptions for control flow: `ReturnException`, `BreakException`, `ContinueException`
- Functions namespaced internally as `ClassName::funcName` to prevent cross-class collisions
- Module functions stored in separate `moduleFunctions` map (C++ built-ins)
- `Value` struct holds all possible type fields simultaneously (~100+ bytes per value)

## Key Behaviors

- **Function resolution**: built-ins → module functions → namespaced (`ClassName::funcName`) → plain name
- **Scope**: stack of unordered_maps; `if`/`while`/`for`/`for-in` each push a scope; exceptions pop before propagating
- **Module dispatch**: `math.*` and `fs.*` checked via `DotCallExpr`/`DotExpr` before object method lookup
- **Imports**: relative to source dir; namespaced as `ClassName::funcName`; circular detection; no transitive imports
- **Truthiness**: `0`, `0.0`, `false`, `""`, `[]` are falsy; everything else truthy
- **Type promotion**: int + float → float; int / int → int (truncates)

## Known Limitations (as of v0.0.14)

- No unary minus (`-x`) — use `0 - x`
- No logical operators (`&&`, `||`, `!`)
- No empty dict literal (`{}`)
- `for-in` only iterates arrays (not dicts or strings)
- No recursion depth limit (deep recursion → C++ stack overflow)
- No user-defined function can override built-ins (`len`, `push`, etc.)
- `input()` doesn't flush prompt or handle EOF
- `math.floor`/`math.ceil`/`math.round` return float, not int
- Dict iteration order is non-deterministic
- No string escape for `\0`, `\r`
- No null/nil type — use `void` or empty string

## Running VDX

```bash
# Build from source
cd vdx
cmake -B build
cmake --build build

# Run
./build/vdx examples/hello.vdx
# or after MSI install:
vdx file.vdx
```

## Build Configuration

- C++17 standard
- Compiler warnings: `/W4` (MSVC), `-Wall -Wextra -Wpedantic` (GCC/Clang)
- Source files: `main.cpp`, `lexer.cpp`, `parser.cpp`, `interpreter.cpp`, `modules/fs.cpp`, `modules/math.cpp`
- CPack configured for Windows MSI installer via WiX
- Installer optionally adds `vdx` to system PATH

## References

- See `references/internals.md` for token types, AST nodes, value system, scope system, function resolution, module dispatch, import system, and error handling details
- See `examples/examples.vdx` for working code demonstrating internal behaviors, edge cases, type promotion, truthiness, scope handling, const enforcement, and module dispatch
