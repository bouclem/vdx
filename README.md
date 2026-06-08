# VDX — The Voidware Programming Language

A fast, safe programming language built for AI and games. By [Voidware](https://voidware.xyz).

## What's in this repo

This monorepo contains two projects:

| Folder | What it is |
|--------|------------|
| `vdx/` | The VDX language — lexer, parser, and interpreter written in C++17 |
| `website/` | The official website ([voidwarelang.xyz](https://voidwarelang.xyz)) — Next.js, deployed on Vercel |

## VDX Language (`vdx/`)

VDX is a class-based, interpreted language inspired by Java, C++, Rust, Python, and C#.

**Current version: 0.0.9**

### Features
- `class` declarations — all code lives inside classes
- `let` variables (string, integer, float, bool) with reassignment
- `const` constants (cannot be reassigned)
- Optional type annotations: `let x: int = 5;`, `let pi: float = 3.14;`, `const PI: float = 3.14;`
- `fn` functions with parameters and `return`
- `print()` with expression evaluation
- Operators: `+` `-` `*` `/` `==` `!=` `<` `>` `<=` `>=`
- String concatenation with `+`
- `this` keyword for class-scope access
- `if` / `elif` / `else` control flow
- `while` loops with built-in safety protection
- `for` loops (C-style): `for (let i = 0; i < n; i = i + 1) { ... }`
- `for-in` loops over arrays: `for (item in arr) { ... }`
- Arrays: `[1, 2, 3]`, index access/assignment, `len()`, `push()`
- Objects: `new ClassName()`, dot access (`obj.field`), dot methods (`obj.method()`)
- Types: `float` literals, `true`/`false` booleans, runtime type checking
- Mixed int/float arithmetic with auto-promotion
- `wait(ms)` to pause execution
- `@unsafe` annotation to bypass loop protection
- `break` statement for exiting loops early
- `continue` statement for skipping to next loop iteration
- `math` module with functions: `sqrt`, `pow`, `abs`, `sin`, `cos`, `tan`, `floor`, `ceil`, `round`, `min`, `max`, `random`, `pi`
- Block scoping
- Improved error reporting with source context

### Quick start

```bash
cd vdx
cmake -B build
cmake --build build
./build/vdx examples/hello.vdx
```

### Install (Windows)

Download the `.msi` installer from [voidwarelang.xyz/download](https://voidwarelang.xyz/download). After installation, `vdx` is available in your terminal:

```bash
vdx myfile.vdx
```

### Example

```
class Hello {
    let name: string = "VDX";
    const VERSION: float = 0.9;
    print("Welcome to", this.name, "v", VERSION);

    fn max(a, b) {
        if (a > b) { return a; }
        else { return b; }
    }

    print("max(3, 7) =", max(3, 7));

    // Arrays and for-in with break
    let langs = ["Java", "C++", "Rust", "Python"];
    for (lang in langs) {
        if (lang == "Rust") {
            break;  // Stop when we find Rust
        }
        print("Inspired by:", lang);
    }

    // C-style for loop with continue
    @unsafe for (let i = 0; i < 5; i = i + 1) {
        if (i == 2) {
            continue;  // Skip iteration 2
        }
        print("i:", i);
    }

    // Math module usage
    print("Square root of 16:", math.sqrt(16));
    print("Random number:", math.random(1, 100));
}
```

### Docs

Full language documentation is in `vdx/docs/` and on the website at [voidwarelang.xyz/docs](https://voidwarelang.xyz/docs).

- `vdx/docs/README.md` — Language reference
- `vdx/docs/CHANGELOG.md` — Version history
- `vdx/docs/TODO.md` — Roadmap

## Website (`website/`)

The official VDX website, built with Next.js 15 + Tailwind CSS 4.

### Pages
- `/` — Home (hero, code preview, features)
- `/download` — Download the latest `.msi` installer + version archive
- `/docs` — Full language documentation (16 pages covering every feature)

### Setup

```bash
cd website
npm install
npm run dev
```

### Deploy

The site is configured for Vercel. Push to main and it deploys automatically.

Domain: `voidwarelang.xyz`

## Repo structure

```
.
├── vdx/                    # VDX language
│   ├── src/                # C++ source (lexer, parser, interpreter)
│   │   └── modules/        # Built-in modules (math, etc.)
│   ├── examples/           # Example .vdx programs
│   ├── assets/             # Icon (SVG)
│   ├── installer/          # WiX MSI installer config
│   ├── docs/               # Language docs (README, CHANGELOG, TODO)
│   ├── CMakeLists.txt      # Build config
│   └── LICENSE.txt         # MIT
├── website/                # Official website (Next.js)
│   ├── src/app/            # Pages and layouts
│   ├── src/components/     # Shared components
│   └── package.json
├── .gitignore
└── README.md               # This file
```

## License

MIT — see `vdx/LICENSE.txt`
