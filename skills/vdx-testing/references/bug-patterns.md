# VDX Bug Patterns

## Missing `@unsafe` on Legitimate Loop
```vdx
// BUG: loop with slow body killed after 2s
for (let i = 0; i < 1000000; i++) {
    // heavy computation
}

// FIX: add @unsafe
@unsafe for (let i = 0; i < 1000000; i++) {
    // heavy computation
}
```

## Integer Division Surprise
```vdx
let avg = total / count;  // BUG: integer division truncates
let avg = total * 1.0 / count;  // FIX: force float
```

## Module Name Shadowing (v0.1.5+)
```vdx
let math = new Helper();
print(math.sqrt(16));  // BUG: "Undefined method 'sqrt' on Helper" — variable wins over module
// FIX: rename the variable — mu, filesys, plot, etc.
let mu = new Helper();
print(math.sqrt(16));  // now reaches the module
```

## Shared Array/Dict Storage (v0.1.5+)
```vdx
let a = [1, 2, 3];
let b = a;
push(b, 4);
print(a);  // [1, 2, 3, 4] — NOT a bug: arrays/dicts are shared on assign
// FIX (if you need a copy): rebuild element-by-element
let copy = [];
for (v in a) { push(copy, v); }
```

## Dict Key Access on Missing Key
```vdx
let d = {"a": 1};
let x = d["b"];  // BUG: runtime error, key not found
// FIX: check with len or known keys first
```

## for-in on Strings and Dicts (v0.1.5+)
```vdx
let s = "hello";
for (ch in s) { print(ch); }   // works: iterates characters h e l l o

let d = {"b": 2, "a": 1};
for (k in d) { print(k); }     // works: iterates keys in SORTED order → a, b
// (not insertion order — don't rely on it)
```

## Method Name Collision (pre-v0.0.14)
```vdx
class A { fn getName() { return "A"; } }
class B { fn getName() { return "B"; } }  // BUG: duplicate function error (pre-v0.0.14)
// Fixed in v0.0.14: methods namespaced as ClassName::funcName
```

## Transitive Import Missing
```vdx
// file: main.vdx
import "a.vdx";  // a.vdx imports "b.vdx"
// BUG: b.vdx's classes/functions not available here
// FIX: import "b.vdx"; directly
```

## String Concatenation with Non-String Variable
```vdx
let count = 5;
print("count: " + count);  // BUG: "Invalid operator '+' for given types"
// FIX: use comma-separated print args, or str() (v0.1.5+)
print("count:", count);
print("count: " + str(count));
```

## Nested fn Declaration (v0.1.5+)
```vdx
if (x > 0) {
    fn helper() { return 1; }  // BUG: parse error — fn can't nest in blocks
}
// FIX: declare functions at top level or in class bodies
fn helper() { return 1; }
if (x > 0) { helper(); }
```

## Imported Class Methods Aren't Global
```vdx
// utils.vdx contains: class Utils { fn add(a,b) { return a+b; } }
import "utils.vdx";
let r = add(1, 2);            // BUG: "Undefined function 'add'"
// FIX: instantiate the imported class
let u = new Utils();
let r = u.add(1, 2);
// (top-level fns in the imported file CAN be called directly)
```
