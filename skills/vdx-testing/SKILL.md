---
name: vdx-testing
description: Bug hunting, test design, and verification strategies for VDX programs. Use when finding bugs, writing tests, or verifying VDX code correctness. Includes edge case checklists and common bug patterns.
---

# VDX Testing — Bug Hunting & Test Verification

## When to Use

- Finding bugs in VDX programs
- Writing tests or test suites for VDX code
- Verifying VDX code correctness after changes
- Hunting for edge cases and runtime issues

## Bug Hunting Methodology

### Phase 1: Static Analysis (Read Code)

Scan VDX source for these common bug categories:

#### 1. Const Violations
Check all mutation paths on `const` variables:
- Direct reassignment: `const x = 5; x = 10;` — caught at runtime
- `push(constArr, val)`, `pop(constArr)` — caught at runtime
- `constArr[i] = val`, `constObj.field = val` — caught at runtime
- `constCounter++` — caught at runtime

#### 2. Scope Leaks
- `return` inside `if`/`while`/`for`/`for-in` — OK (scope popped before propagating)
- `break`/`continue` at top level or in class body outside a loop — now handled gracefully (v0.0.15+), previously crashed

#### 3. Type Confusion
- Integer division truncation: `7 / 2` yields `3`, not `3.5`
- `math.floor(3.7)` returns `3` (int), `math.round(3.5)` returns `4` (int) — all rounding functions return int (v0.0.15+)
- String indexing returns single-character string, not char/int

#### 4. Module Name Shadowing (v0.1.5+)
- A variable named `math`, `fs`, or `graph` shadows the built-in module
- `let math = new MathUtils();` then `math.sqrt(16)` → "Undefined method 'sqrt' on MathUtils" — the variable wins
- Legacy code that relied on `math.*` hitting the module despite a `math` variable breaks — rename the variable

#### 5. Reference Semantics Surprises (v0.1.5+)
- `let b = arr; push(b, x)` mutates `arr` too — shared storage
- Functions mutating an array param mutate the caller's array
- `arr[i][j] = v`, `obj.field[i] = v`, `d["new"] = v` all work now — check code that assumed they'd fail

#### 6. Import Issues
- Transitive imports not processed — import each file directly
- Import path is relative to source file directory
- Circular imports are detected and skipped cleanly (including back through the entry file, v0.1.5+)
- Imported class methods are NOT global functions — `import` + `add(1,2)` fails if `add` lives inside `class Utils`; use `new Utils()` + `u.add(1,2)`
- Errors inside imported files report the imported file's name/lines (v0.1.5+)

#### 7. Loop Safety (v0.1.5+)
- Any iteration >2000ms triggers error (all loop types incl. `for-in`)
- Any loop exceeding 1,000,000 iterations triggers error — `while(true) {}` now dies
- `wait()`/`input()` blocking time is excluded from the per-iteration budget
- `@unsafe` before the loop disables both checks

#### 8. Array/String Bounds
- Negative indices not allowed: `arr[-1]` throws
- Index >= length throws; `pop()` on empty array throws
- `d["missing"]` read throws, but `d["missing"] = v` CREATES the key (v0.1.5+)
- `s[i] = "c"` mutates a string char in place (v0.1.5+) — strings are no longer read-only

#### 9. Object Method Issues
- Methods can call sibling methods by bare name within class body
- Method-to-method calls resolve via `currentClassName::funcName`
- `new ClassName()` runs only `let` field initializers; other class-body statements execute once at class LOAD time (v0.1.5+)
- `this.x = v` inside methods persists (v0.1.5+; pre-0.1.5 it was silently reverted for pre-existing fields)
- `this` does NOT leak into plain functions called from methods (v0.1.5+)

#### 10. Parser-Rejected Constructs (v0.1.5+ — previously silent dead code or confusing errors)
- `fn` inside `if`/loops/`fn` bodies → parse error (was silently dropped)
- `break`/`continue` outside a loop → parse error
- Recursion beyond 500 calls → runtime error (was segfault)

### Phase 2: Dynamic Testing (Run Code)

#### Test File Structure
Starting with v0.0.15, test files can use top-level statements directly (no class wrapper required):
```vdx
// Setup
let input = [1, 2, 3];

// Test case
let result = len(input);
print("len([1,2,3]):", result);

// Assertion (manual — VDX has no assert)
if (result == 3) {
    print("PASS: len test");
} else {
    print("FAIL: len test, expected 3, got", result);
}
```

#### Multi-class Test Pattern
```vdx
class Helper {
    fn add(a, b) {
        return a + b;
    }
}

class TestAdd {
    let r1 = add(2, 3);
    if (r1 == 5) { print("PASS: add(2,3)"); } else { print("FAIL: add(2,3) =", r1); }
    
    let r2 = add(0, 0);
    if (r2 == 0) { print("PASS: add(0,0)"); } else { print("FAIL: add(0,0) =", r2); }
    
    let r3 = add(0 - 5, 5);
    if (r3 == 0) { print("PASS: add(-5,5)"); } else { print("FAIL: add(-5,5) =", r3); }
}
```

#### Error Case Testing
Test that expected errors actually throw:
```vdx
class TestErrors {
    // Test division by zero
    fn testDivZero() {
        let x = 1 / 0;  // should throw
        print("FAIL: should have thrown");
        return 0;
    }
    
    // If we reach here, the error wasn't caught
    // VDX has no try/catch, so errors terminate the program
    // Error tests must be run as separate files
}
```

Since VDX has no try/catch, error cases must be tested by running separate files and checking exit code + stderr.

### Phase 3: Edge Case Checklist

See `references/edge-cases.md` for the full checklist covering numbers, strings, arrays, dicts, control flow, objects, const, imports, and modules.

### Phase 4: Regression Test Suite

Create test files in `examples/` following naming convention:
- `test-<feature>.vdx` — feature tests
- `test-bug<N>.vdx` — regression tests for specific bugs
- `test-<version>.vdx` — version-specific feature tests

#### Running Tests
```bash
# Single test
./build/vdx examples/test-feature.vdx

# Batch runner (PowerShell)
Get-ChildItem examples/test-*.vdx | ForEach-Object {
    Write-Host "Running $_..." -NoNewline
    $result = ./build/vdx $_.FullName 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Host " PASS" -ForegroundColor Green
    } else {
        Write-Host " FAIL" -ForegroundColor Red
        Write-Host $result
    }
}
```

#### Test Output Convention
- Print `PASS: <test name>` for passing assertions
- Print `FAIL: <test name>, expected X, got Y` for failures
- Exit code 0 = all passed, exit code 1 = error/failure
- Error tests: expect non-zero exit code and specific error message

## Bug Report Template

```
### BUG-<N>: <short title>
**Severity:** CRITICAL / HIGH / MEDIUM / LOW
**File:** <filename>:<line>
**Description:** <what's wrong>
**Repro:** <minimal VDX code to reproduce>
**Expected:** <what should happen>
**Actual:** <what actually happens>
**Root cause:** <which interpreter/parser/lexer mechanism is responsible>
```

## References

- See `references/edge-cases.md` for the full edge case checklist
- See `references/bug-patterns.md` for common VDX bug patterns with repro and fix examples

## Example Code

See these files in the `examples/` directory:

- **`examples/examples.vdx`** — Full test suite with Assert helper, MathUtils, BankAccount, and regression tests. All tests pass. Run with:
  ```bash
  vdx examples/examples.vdx
  ```

- **`examples/test-error-*.vdx`** — Error case tests (8 files). Each should fail with a specific error message. Run individually:
  ```bash
  vdx examples/test-error-const-assign.vdx    # "Cannot assign to const variable"
  vdx examples/test-error-const-push.vdx      # "Cannot push to const array"
  vdx examples/test-error-break-method.vdx    # "'break' used outside of a loop"
  vdx examples/test-error-div-zero.vdx        # "Division by zero"
  vdx examples/test-error-index-oob.vdx       # "Array index out of bounds"
  vdx examples/test-error-nested-fn.vdx       # parse error: fn nested in a block (v0.1.5+)
  vdx examples/test-error-int-overflow.vdx    # "Integer literal is out of range"
  vdx examples/test-error-pop-empty.vdx       # "pop() cannot pop from empty array"
  ```
- New error cases to cover (v0.1.5): `INT_MIN / -1` (int overflow error, not crash), recursion >500 calls, loop >1M iterations, `fn` nested in a block (parse error), `fs.setRoot()` sandbox escapes, `this` outside method context, `math.fibonacci(47)` (out-of-range guard), imported-file error attribution
