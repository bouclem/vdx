# VDX Edge Case Checklist

## Numbers
- [ ] Zero: `0`, `0.0`
- [ ] Large numbers: near INT_MAX
- [ ] Negative numbers: `-x`, `0 - x`
- [ ] Literal forms (v0.1.5+): `1e5`, `1.5e-3`, `0xFF`, `1.`, `.5`
- [ ] Float precision: `0.1 + 0.2`
- [ ] Integer overflow: operations near INT_MAX
- [ ] Division edge: `1 / 0`, `1 % 0`, `INT_MIN / -1` (v0.1.5+: clean error, used to crash)
- [ ] Mixed int/float: `1 + 2.5`, `3 / 2` vs `3 / 2.0`
- [ ] `math.fibonacci(47)` → out-of-range error (v0.1.5+; fib(47) > INT_MAX)
- [ ] `math.sum`/`min`/`max` on values overflowing int → error (v0.1.5+)

## Strings
- [ ] Empty string: `""`
- [ ] Escapes: `"\n"`, `"\t"`, `"\\", "\""`, `"\r"`, `"\0"`
- [ ] Concatenation: `"a" + "b"`; `"a" + n` throws — use `str(n)` (v0.1.5+)
- [ ] Index: `s[0]`, `s[len(s) - 1]`; index ASSIGN `s[0] = "x"` (v0.1.5+)
- [ ] Out of bounds: `s[999]`
- [ ] Comparisons (v0.1.5+): `"a" < "b"`, `<=`, `>`, `>=` — lexicographic
- [ ] `for (ch in s)` iterates characters (v0.1.5+)
- [ ] Builtins (v0.1.5+): `split`, `substr`, `indexOf`, `upper`, `lower`, `trim`, `replace`, `join`

## Arrays
- [ ] Empty: `[]`
- [ ] Single element: `[1]`
- [ ] Mixed types: `[1, "two", true]`
- [ ] Nested: `[[1, 2], [3, 4]]`; nested assign `m[0][1] = 9` (v0.1.5+)
- [ ] push/pop on empty; `push(obj.field, v)` lvalue args (v0.1.5+)
- [ ] Index out of bounds
- [ ] Negative index
- [ ] Reference semantics (v0.1.5+): `let b = a; push(b, x)` → `a` changed too

## Dicts
- [ ] Single entry: `{"k": 1}`
- [ ] Empty literal: `{}` (supported since v0.1.1)
- [ ] Multiple entries
- [ ] Key access: `d["k"]`
- [ ] Key assignment creates keys: `d["new"] = val` (v0.1.5+)
- [ ] Non-string key (should error)
- [ ] Missing key READ (should error)
- [ ] `for (k in d)` iterates sorted keys (v0.1.5+)
- [ ] `d.k` does NOT work — only `d["k"]`

## Control Flow
- [ ] `if` with no elif/else
- [ ] `elif` chain
- [ ] Nested if
- [ ] `while` with break
- [ ] `while` with continue
- [ ] `for` with break
- [ ] `for` with continue
- [ ] `for-in` with break
- [ ] `for-in` with continue
- [ ] `return` inside if/while/for/for-in
- [ ] `break`/`continue` outside a loop → parse error (v0.1.5+)
- [ ] `fn` inside `if`/loop/`fn` body → parse error (v0.1.5+; was silently dropped)
- [ ] `return` at top level (graceful in v0.0.15+)
- [ ] `return` in class body outside function (graceful in v0.0.15+)
- [ ] Logical ops (v0.1.5+): `a && b`, `a || b`, `!x`, short-circuit (right side skipped when decided)
- [ ] Loop safety: >2s/iteration AND >1M total iterations both error (v0.1.5+); `wait`/`input` blocking excluded
- [ ] Recursion depth >500 → error (v0.1.5+); parser expression depth bounded

## Objects
- [ ] `new` with no fields
- [ ] `new` with field initializers
- [ ] Method calling sibling method
- [ ] Method modifying fields — `x = v` and `this.x = v` both persist (v0.1.5+)
- [ ] `this` used outside method → error (v0.1.5+)
- [ ] Two classes with same method name
- [ ] Object field assignment; `obj.f[i] = v`, `obj.n++` (v0.1.5+)
- [ ] Object as function argument
- [ ] Class body executes at load (v0.1.5+ restored); `new` only runs `let` initializers

## Const
- [ ] Direct reassignment
- [ ] push on const array
- [ ] pop on const array
- [ ] Index assign on const array
- [ ] Dot assign on const object
- [ ] ++/-- on const variable

## Imports
- [ ] Import with top-level fn calls (direct call)
- [ ] Import with class — `new` + dot-call (methods are NOT global)
- [ ] Circular import — incl. back through the entry file (v0.1.5+: clean skip)
- [ ] Missing import file
- [ ] Same-named methods in imported and local classes
- [ ] Error inside imported file reports imported filename/lines (v0.1.5+)

## Modules
- [ ] `math.sqrt(0)`, `math.sqrt(0 - 1)` (negative)
- [ ] `math.pow(2, 10)`, `math.pow(0, 0)`
- [ ] `math.random()` range [0, 1)
- [ ] `math.random(10)` range [0, 10] (inclusive, v0.0.15+)
- [ ] `math.random(1, 5)` range [1, 5]
- [ ] `math.round(3.5)` returns int 4 (not float, v0.0.15+)
- [ ] `math.floor(3.7)` returns int 3 (not float, v0.0.15+)
- [ ] `math.ceil(3.2)` returns int 4 (not float, v0.0.15+)
- [ ] `math.pi` value
- [ ] `math.sort`/`sortDesc` on non-numeric arrays → error (v0.1.5+)
- [ ] `math.abs(INT_MIN)` → error (v0.1.5+)
- [ ] Variable named `math`/`fs`/`graph` shadows the module (v0.1.5+)
- [ ] `fs.readFile` on existing file
- [ ] `fs.readFile` on missing file
- [ ] `fs.writeFile` and read back
- [ ] `fs.append`, `fs.exists`, `fs.listDir` (v0.1.5+)
- [ ] `fs.setRoot` then `..` escape → sandbox error (v0.1.5+)
- [ ] `wait(0.5)` float duration (v0.1.5+)
- [ ] `graph.color` validates input — `[a-zA-Z0-9#]` only (v0.1.5+)
