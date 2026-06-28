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
- `break`/`continue` inside a function or method but not in a loop — runtime error

#### 3. Type Confusion
- Integer division truncation: `7 / 2` yields `3`, not `3.5`
- `math.floor(3.7)` returns `3.0` (float), not `3` (int)
- String indexing returns single-character string, not char/int

#### 4. Missing Language Features
- Unary minus not supported: `-5` is a parse error
- No logical operators: `&&`, `||`, `!` are parse errors
- Empty dict `{}` is a parse error
- `for-in` only works on arrays, not dicts or strings

#### 5. Import Issues
- Transitive imports not processed — import each file directly
- Import path is relative to source file directory
- Circular imports are detected and throw clean error

#### 6. Loop Safety
- `while` and `for` iterations >2000ms trigger error
- `for-in` has NO safety check — can hang on large arrays
- `@unsafe` disables the check for `while` and `for` only

#### 7. Array/String Bounds
- Negative indices not allowed: `arr[-1]` throws
- Index >= length throws; `pop()` on empty array throws

#### 8. Object Method Issues
- Methods can call sibling methods by bare name within class body
- Method-to-method calls resolve via `currentClassName::funcName`
- `new ClassName()` executes all non-function statements in class body as constructor

### Phase 2: Dynamic Testing (Run Code)

#### Test File Structure
All VDX test files must wrap code in a class:
```vdx
class TestSomething {
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
  vdx examples/test-error-unary-minus.vdx     # "Expected expression" (got '-')
  vdx examples/test-error-int-overflow.vdx    # "Integer literal is out of range"
  vdx examples/test-error-pop-empty.vdx       # "pop() cannot pop from empty array"
  ```
