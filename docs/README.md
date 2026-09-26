# VDX Programming Language

The programming language of **Voidware** ([voidware.xyz](https://voidware.xyz)).

Inspired by Java, C++, Rust, Python, and C# — built to be fast, with future focus on AI and games.

## Version: 0.1.5

### Supported Features
- `class` declarations (recommended but optional — top-level statements work without a class wrapper)
- `print()` with any expression arguments (e.g., `print(1 + 1)` outputs `2`)
- `let` variable declarations (string, integer, float, bool)
- Optional type annotations: `let x: int = 5;`, `let pi: float = 3.14;`
- Variable reassignment (`name = expr;`)
- `fn` / `func` function declarations with parameters and `return`
- Operators: `+`, `-`, `*`, `/`, `%`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `++`, `--`, `!`, `&&`, `||` (short-circuit)
- String concatenation with `+`; string comparisons with `<`, `>`, `<=`, `>=`
- Parenthesized expressions
- `this` keyword for class-scope variable access
- `if` / `elif` / `else` control flow
- `while` loop
- `for` loop (C-style): `for (let i = 0; i < n; i++) { ... }` (supports `++`/`--` in update)
- `for-in` loop over arrays (elements), strings (characters), and dicts (sorted keys): `for (item in arr) { ... }`
- Block scoping (variables in `{ }` blocks are local)
- `wait(ms)` to pause execution (int or float: `wait(0.5)`)
- **Loop safety protection** — any loop is halted if an iteration takes more than 2 seconds or the loop exceeds 1,000,000 iterations (`wait`/`input` blocking time is not counted)
- **`@unsafe` annotation** — place before `while`, `for`, or `for-in` to disable loop protection
- **Types** — `float` literals (`3.14`), `true`/`false` booleans, type annotations with runtime checking
- **Mixed arithmetic** — int/float operations auto-promote to float
- **`new` / object instantiation** — `let obj = new ClassName();`
- **Dot access** — `obj.field`, `obj.method(args)`, `obj.field = value;`
- **Arrays** — `let arr = [1, 2, 3];`, index access `arr[0]`, index assignment `arr[0] = 5;`, nested assign `m[i][j] = v;`
- **Reference semantics** — `let b = arr` shares storage; mutations through `b` are visible through `arr`
- **Dicts** — `let d = {"k": v};`, access `d["k"]`, assign `d["new"] = v` (creates keys)
- **Generalized lvalues** — `obj.field[i] = v`, `this.arr[i] = v`, `s[i] = "c"`, `arr[i]++`, `obj.n--`
- **Built-in `len()`** — returns length of arrays, strings, dicts, and objects
- **Built-in `push()`** — appends a value to an array: `push(arr, 4);` (also `push(obj.items, x)`)
- **Built-ins** — `int()`, `float()`, `str()` conversions; `split`, `substr`, `indexOf`, `upper`, `lower`, `trim`, `replace`, `join` string functions
- **Number literals** — `42`, `0xFF`, `3.14`, `1.`, `.5`, `1e5`, `1.5e-3`
- **`break`** — exit loops early
- **`continue`** — skip to next loop iteration
- **`const`** — declare immutable constants: `const PI = 3.14;`
- **`math` module** — math functions: `sqrt`, `pow`, `abs`, `sin`, `cos`, `tan`, `floor`, `ceil`, `round`, `min`, `max`, `random`, `pi`, `log`, `log2`, `log10`, `exp`, `cbrt`, `asin`, `acos`, `atan`, `atan2`, `degrees`, `radians`, `gcd`, `sign`, `clamp`, `factorial`, `fibonacci`, `isPrime`, `primes`, `primeCount`, `sort`, `sortDesc`, `count`, `lcm`, `sum`, `mean`, `comb`, `hypot`, `lerp`, `e`, `tau`
- **`graph` module** — SVG plotting: `graph.scatter(xs, ys)`, `graph.line(xs, ys)`, `graph.bar(labels, values)`, `graph.hist(data, [bins])`, `graph.area(xs, ys)`, `graph.title()`, `graph.xlabel()`, `graph.ylabel()`, `graph.grid(bool)`, `graph.color(name)`, `graph.legend(labels)`, `graph.save(path)`, `graph.show()`
- **`fs` module** — file I/O: `fs.readFile`, `fs.writeFile`, `fs.append`, `fs.exists`, `fs.listDir`, `fs.setRoot` (opt-in sandbox)
- **`import`** — import other VDX files: `import "utils.vdx";` (top-level fns callable directly; class methods via `new` + dot-call)
- **CLI** — `vdx --help`, `vdx --version`
- **`type()`** — get type name as string: `type(42)` returns `"int"`
- **`input()`** — read user input: `let name = input("Name: ");`
- **`pop()`** — remove and return last array element: `let last = pop(arr);`
- **Extended `len()`** — now works with objects (returns field count)
- **Improved error reporting** — errors now show source file, line number, and surrounding code context

### Loop Safety
By default, VDX halts a loop when an iteration takes more than 2000ms or the loop runs more than 1,000,000 iterations — this catches both slow iterations and fast infinite loops like `while(true) {}`. Time blocked in `wait()` and `input()` does not count against the per-iteration budget.

To bypass this for legitimate slow or long-running loops:
```vdx
@unsafe while (condition) {
    // runs without safety checks
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
