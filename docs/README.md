# VDX Programming Language

The programming language of **Voidware** ([voidware.xyz](https://voidware.xyz)).

Inspired by Java, C++, Rust, Python, and C# — built to be fast, with future focus on AI and games.

## Version: 0.1.4

### Supported Features
- `class` declarations (recommended but optional — top-level statements work without a class wrapper)
- `print()` with any expression arguments (e.g., `print(1 + 1)` outputs `2`)
- `let` variable declarations (string, integer, float, bool)
- Optional type annotations: `let x: int = 5;`, `let pi: float = 3.14;`
- Variable reassignment (`name = expr;`)
- `fn` / `func` function declarations with parameters and `return`
- Operators: `+`, `-`, `*`, `/`, `%`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `++`, `--`
- String concatenation with `+`
- Parenthesized expressions
- `this` keyword for class-scope variable access
- `if` / `elif` / `else` control flow
- `while` loop
- `for` loop (C-style): `for (let i = 0; i < n; i++) { ... }` (supports `++`/`--` in update)
- `for-in` loop over arrays: `for (item in arr) { ... }`
- Block scoping (variables in `{ }` blocks are local)
- `wait(ms)` to pause execution
- **Loop safety protection** — `while` and `for` loops with iterations taking more than 2 seconds are flagged as potentially infinite
- **`@unsafe` annotation** — place before `while` or `for` to disable loop protection
- **Types** — `float` literals (`3.14`), `true`/`false` booleans, type annotations with runtime checking
- **Mixed arithmetic** — int/float operations auto-promote to float
- **`new` / object instantiation** — `let obj = new ClassName();`
- **Dot access** — `obj.field`, `obj.method(args)`, `obj.field = value;`
- **Arrays** — `let arr = [1, 2, 3];`, index access `arr[0]`, index assignment `arr[0] = 5;`
- **Built-in `len()`** — returns length of arrays and strings
- **Built-in `push()`** — appends a value to an array: `push(arr, 4);`
- **`break`** — exit loops early
- **`continue`** — skip to next loop iteration
- **`const`** — declare immutable constants: `const PI = 3.14;`
- **`math` module** — math functions: `sqrt`, `pow`, `abs`, `sin`, `cos`, `tan`, `floor`, `ceil`, `round`, `min`, `max`, `random`, `pi`, `log`, `log2`, `log10`, `exp`, `cbrt`, `asin`, `acos`, `atan`, `atan2`, `degrees`, `radians`, `gcd`, `sign`, `clamp`, `factorial`, `fibonacci`, `isPrime`, `primes`, `primeCount`, `sort`, `sortDesc`, `count`, `lcm`, `sum`, `mean`, `comb`, `hypot`, `lerp`, `e`, `tau`
- **`graph` module** — SVG plotting: `graph.scatter(xs, ys)`, `graph.line(xs, ys)`, `graph.bar(labels, values)`, `graph.hist(data, [bins])`, `graph.area(xs, ys)`, `graph.title()`, `graph.xlabel()`, `graph.ylabel()`, `graph.grid(bool)`, `graph.color(name)`, `graph.legend(labels)`, `graph.save(path)`, `graph.show()`
- **`import`** — import other VDX files: `import "utils.vdx";`
- **`type()`** — get type name as string: `type(42)` returns `"int"`
- **`input()`** — read user input: `let name = input("Name: ");`
- **`pop()`** — remove and return last array element: `let last = pop(arr);`
- **Extended `len()`** — now works with objects (returns field count)
- **Improved error reporting** — errors now show source file, line number, and surrounding code context

### Loop Safety
By default, if a `while` or `for` loop iteration takes more than 2000ms, VDX halts it with an error. This prevents infinite loops and runaway CPU usage.

To bypass this for legitimate slow loops:
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

    // for loop
    @unsafe for (let i = 0; i < 3; i++) {
        print("i:", i);
    }

    // for-in
    let fruits = ["apple", "banana"];
    for (fruit in fruits) {
        print("fruit:", fruit);
    }

    // types
    let pi: float = 3.14;
    let active: bool = true;
    print("pi:", pi, "active:", active);
}
```

### Object Instantiation
```vdx
class Point {
    let x: int = 0;
    let y: int = 0;

    fn setXY(nx, ny) {
        x = nx;
        y = ny;
    }

    fn describe() {
        print("Point:", x, y);
    }
}

class Main {
    let p = new Point();
    p.x = 10;
    p.y = 20;
    p.describe();
}
```

### Build
```bash
cd vdx
cmake -B build
cmake --build build
```

### Build Release (Windows MSI)
```powershell
# From the repository root — builds, creates MSI installer, and copies to releases/
.\build-release.ps1
```

### Run
```bash
./build/vdx examples/hello.vdx
```

### File Format
`.vdx`
