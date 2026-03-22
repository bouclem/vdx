# VDX Programming Language

The programming language of **Voidware** ([voidware.xyz](https://voidware.xyz)).

Inspired by Java, C++, Rust, Python, and C# — built to be fast, with future focus on AI and games.

## Version: 0.0.6

### Supported Features
- `class` declarations
- `print()` with any expression arguments (e.g., `print(1 + 1)` outputs `2`)
- `let` variable declarations (string, integer)
- Variable reassignment (`name = expr;`)
- `fn` function declarations with parameters and `return`
- Operators: `+`, `-`, `*`, `/`, `==`, `!=`, `<`, `>`, `<=`, `>=`
- String concatenation with `+`
- Parenthesized expressions
- `this` keyword for class-scope variable access
- `if` / `elif` / `else` control flow
- `while` loop
- Block scoping (variables in `{ }` blocks are local)
- `wait(ms)` to pause execution
- **Loop safety protection** — `while` loops that iterate in under 2 seconds are blocked by default
- **`@unsafe` annotation** — place before `while` to disable loop protection

### Loop Safety
By default, if a `while` loop iteration completes in less than 2000ms, VDX halts it with an error. This prevents infinite loops and runaway CPU usage.

To bypass this for legitimate fast loops:
```vdx
@unsafe while (condition) {
    // runs without speed checks
}
```

### Install (Windows)
Download the `.msi` installer from [voidwarelang.xyz/download](https://voidwarelang.xyz/download).
After installation, `vdx` is available in your terminal PATH.

### Example (`hello.vdx`)
```vdx
class Hello {
    print(1 + 1);

    let name = "VDX";
    print(this.name);

    let x = 10;
    if (x == 10) {
        print("x is 10");
    } elif (x > 5) {
        print("x is greater than 5");
    } else {
        print("x is something else");
    }

    fn max(a, b) {
        if (a > b) {
            return a;
        } else {
            return b;
        }
    }

    print("max(3, 7) =", max(3, 7));

    // while loop
    fn countdown(n) {
        while (n > 0) {
            print("countdown:", n);
            n = n - 1;
        }
        print("done!");
    }

    countdown(3);

    // wait
    wait(100);
    print("done waiting!");
}
```

### Build
```bash
cd vdx
cmake -B build
cmake --build build
```

### Run
```bash
./build/vdx examples/hello.vdx
```

### File Format
`.vdx`
