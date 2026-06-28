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

## Conditional Without Logical Operators

```vdx
// Instead of: if (a && b)
if (a) {
    if (b) {
        // both true
    }
}

// Instead of: if (a || b)
let either = 0;
if (a) { either = 1; }
if (b) { either = 1; }
if (either == 1) {
    // at least one true
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
class Utils {
    fn add(a, b) {
        return a + b;
    }

    fn isEven(n) {
        return n % 2 == 0;
    }

    fn clamp(val, minVal, maxVal) {
        if (val < minVal) { return minVal; }
        if (val > maxVal) { return maxVal; }
        return val;
    }
}
```

## Negative Numbers

```vdx
// VDX has no unary minus — use binary subtraction
let neg = 0 - 42;
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

For larger programs, wrapping in `class{}` is still recommended for organization.
