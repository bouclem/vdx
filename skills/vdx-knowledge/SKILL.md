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
- **Repository**: bouclem/vdx
- **File extension**: `.vdx`
- **Current version**: 0.1.5

## Architecture

### Pipeline
1. **Lexer** (`lexer.cpp`) — Tokenizes source into tokens with line/column tracking
2. **Parser** (`parser.cpp`) — Recursive descent parser, builds AST with `shared_ptr` nodes
3. **Interpreter** (`interpreter.cpp`) — Tree-walking evaluator with stack-based scopes

### Key Design Decisions
- `class{}` wrapper is optional (v0.0.15+) — top-level statements and functions allowed at file scope
- `shared_ptr` for AST node ownership
- Stack of `unordered_map` scopes for variable lookup
- Exceptions for control flow: `ReturnException`, `BreakException`, `ContinueException`
- Functions namespaced internally as `ClassName::funcName` to prevent cross-class collisions
- Module functions stored in separate `moduleFunctions` map (C++ built-ins)
- `Value` struct holds all type fields; arrays/dicts use `shared_ptr` storage (reference semantics, v0.1.5+)

## Key Behaviors

- **Function resolution**: namespaced (`ClassName::funcName`) → plain user functions → built-ins (`len`, `push`, …) → module functions (v0.1.5+: user functions shadow built-ins)
- **Scope**: stack of unordered_maps; `if`/`while`/`for`/`for-in` each push a scope; exceptions pop before propagating
- **Module dispatch**: `math.*`/`fs.*`/`graph.*` only reached when the name is not bound to a variable — a user variable named `math`/`fs`/`graph` wins (v0.1.5+)
- **Class bodies execute**: `fn` registers, `let` runs its initializer (also at `new`), other statements run once at load — the `class Main { <program> }` wrapper idiom works (restored in v0.1.5)
- **Reference semantics**: `let b = a` shares array/dict storage — mutations via `b` are visible through `a` (v0.1.5+)
- **Imports**: relative to source dir; classes registered, methods namespaced `ClassName::funcName`, top-level fns plain; entry file marked imported (circular through main is now a clean skip); no transitive imports; imported methods need `new` + dot-call
- **Safety guards (v0.1.5+)**: call depth ≤ 500, loop iterations ≤ 1,000,000 per loop (plus >2s/iteration check; `wait`/`input` blocking time excluded), parser expression depth bounded
- **Truthiness**: `0`, `0.0`, `false`, `""`, `[]` are falsy; everything else truthy
- **Type promotion**: int + float → float; int / int → int (truncates)

## Known Limitations (as of v0.1.5)

- No dot access on dicts: `d["k"]` works, `d.k` does not (`.field` is for objects/modules)
- `print("x: " + n)` throws — use `print("x:", n)` or `str(n)`
- `input()` doesn't flush prompt or handle EOF
- No null/nil type — use `void` or empty string
- Transitive imports not processed — import each file directly
- Imported class methods are not global functions — use `let u = new Utils(); u.add(a, b)`
- `wait()`/`input()`-only blocking excluded from loop timing, but the 1M-iteration cap still applies (use `@unsafe` for long loops)
- No `try`/`catch` — any runtime error terminates the program

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
vdx --help      # usage (v0.1.5+)
vdx --version   # prints "vdx 0.1.5" (v0.1.5+)
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
