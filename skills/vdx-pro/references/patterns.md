# VDX Code Patterns

## Singleton-like Entry Point

```vdx
class Main {
    let app = new App();
    app.run();
}
```

## Data Class

```vdx
class Point {
    let x: int = 0;
    let y: int = 0;

    fn setXY(nx, ny) {
        x = nx;
        y = ny;
    }

    fn distance(other) {
        let dx = x - other.x;
        let dy = y - other.y;
        return math.sqrt(dx * dx + dy * dy);
    }
}
```

## Collection Iteration

```vdx
// for-in (when index not needed)
let items = [1, 2, 3, 4, 5];
for (item in items) {
    print(item);
}

// Index-based (when index needed)
@unsafe for (let i = 0; i < len(items); i++) {
    print("index", i, "value", items[i]);
}
```

## Logical Conditions (v0.1.5+)

```vdx
if (a && b) {
    // both true — && and || short-circuit
}

if (a || b) {
    // at least one true
}

if (!done) {
    // negation
}
```

## Stack Pattern

```vdx
class Stack {
    let items = [];

    fn pushItem(val) {
        push(items, val);
    }

    fn popItem() {
        if (len(items) == 0) {
            return 0 - 1;
        }
        return pop(items);
    }

    fn peek() {
        if (len(items) == 0) {
            return 0 - 1;
        }
        return items[len(items) - 1];
    }

    fn size() {
        return len(items);
    }
}
```

## Config/Settings Pattern

```vdx
class Config {
    let settings = {"timeout": 30, "retries": 3};

    fn get(key) {
        return settings[key];
    }

    fn set(key, val) {
        settings[key] = val;
    }
}
```

## Utility Module (for import)

```vdx
// utils.vdx — import with: import "utils.vdx";
// Option A: top-level functions — callable directly after import
fn add(a, b) {
    return a + b;
}

fn isEven(n) {
    return n % 2 == 0;
}

// Option B: class of methods — call via an object after import
class Utils {
    fn clamp(val, minVal, maxVal) {
        if (val < minVal) { return minVal; }
        if (val > maxVal) { return maxVal; }
        return val;
    }
}

// Importer side:
//   let x = add(1, 2);              // top-level fn — direct call
//   let u = new Utils();
//   let y = u.clamp(5, 0, 3);       // class method — via object
```

## Negative Numbers

```vdx
// Unary minus works — or use binary subtraction, both fine
let neg = -42;
let absVal = math.abs(neg);
```

## Float Division Trick

```vdx
// Force float result from integer operands
let avg = total * 1.0 / count;
```

## Class-Free Script (v0.0.15+)

```vdx
// No class{} wrapper needed — top-level statements run directly
print("Starting...");

let data = [1, 2, 3, 4, 5];
let sum = 0;
for (n in data) {
    sum = sum + n;
}
print("Sum:", sum);

fn double(x) {
    return x * 2;
}
print("Double of sum:", double(sum));
```

For larger programs, wrapping in `class{}` is still recommended for organization — class bodies execute their statements at load, so `class Main { <program> }` works as an entry point.

## Shared Array/Dict Storage (v0.1.5+)

```vdx
// Arrays and dicts are shared on assignment — pass them "by reference"
let scores = [1, 2, 3];
let alias = scores;
push(alias, 4);
print(scores);  // [1, 2, 3, 4] — alias and scores are the same array

// To copy, rebuild: 
let copy = [];
for (v in scores) { push(copy, v); }

// This is what makes field/nested mutation work:
class Bag {
    let items = [];
}
let b = new Bag();
push(b.items, "rock");   // v0.1.5+ — works on lvalues, not just names
```
