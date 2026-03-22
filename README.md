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

**Current version: 0.0.6**

### Features
- `class` declarations — all code lives inside classes
- `let` variables (string, integer) with reassignment
- `fn` functions with parameters and `return`
- `print()` with expression evaluation
- Operators: `+` `-` `*` `/` `==` `!=` `<` `>` `<=` `>=`
- String concatenation with `+`
- `this` keyword for class-scope access
- `if` / `elif` / `else` control flow
- `while` loops with built-in safety protection
- `wait(ms)` to pause execution
- `@unsafe` annotation to bypass loop protection
- Block scoping

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
    let name = "VDX";
    print("Welcome to", this.name);

    fn max(a, b) {
        if (a > b) { return a; }
        else { return b; }
    }

    print("max(3, 7) =", max(3, 7));

    // Fast loop — requires @unsafe
    fn countdown(n) {
        @unsafe while (n > 0) {
            print(n);
            n = n - 1;
        }
    }

    countdown(5);
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
