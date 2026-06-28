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

## No Unary Minus
```vdx
let x = -5;  // BUG: parse error
let x = 0 - 5;  // FIX: binary subtraction
```

## No Logical Operators
```vdx
if (a && b) { ... }  // BUG: parse error
// FIX: nested if
if (a) {
    if (b) { ... }
}
```

## Dict Key Access on Missing Key
```vdx
let d = {"a": 1};
let x = d["b"];  // BUG: runtime error, key not found
// FIX: check with len or known keys first
```

## for-in on Non-Array
```vdx
let s = "hello";
for (ch in s) { ... }  // BUG: runtime error, for-in requires array
// FIX: use index-based loop
@unsafe for (let i = 0; i < len(s); i++) {
    let ch = s[i];
    // ...
}
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
// FIX: use comma-separated print args
print("count:", count);
```
