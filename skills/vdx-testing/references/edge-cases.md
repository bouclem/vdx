# VDX Edge Case Checklist

## Numbers
- [ ] Zero: `0`, `0.0`
- [ ] Large numbers: near INT_MAX
- [ ] Negative numbers: `0 - x` (no unary minus)
- [ ] Float precision: `0.1 + 0.2`
- [ ] Integer overflow: operations near INT_MAX
- [ ] Division by zero: `1 / 0`, `1 % 0`
- [ ] Mixed int/float: `1 + 2.5`, `3 / 2` vs `3 / 2.0`

## Strings
- [ ] Empty string: `""`
- [ ] Escapes: `"\n"`, `"\t"`, `"\\", "\""`
- [ ] Concatenation: `"a" + "b"`
- [ ] Index: `s[0]`, `s[len(s) - 1]`
- [ ] Out of bounds: `s[999]`

## Arrays
- [ ] Empty: `[]`
- [ ] Single element: `[1]`
- [ ] Mixed types: `[1, "two", true]`
- [ ] Nested: `[[1, 2], [3, 4]]`
- [ ] push/pop on empty
- [ ] Index out of bounds
- [ ] Negative index

## Dicts
- [ ] Single entry: `{"k": 1}`
- [ ] Multiple entries
- [ ] Key access: `d["k"]`
- [ ] Key assignment: `d["new"] = val`
- [ ] Non-string key (should error)
- [ ] Missing key access (should error)

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
- [ ] `break`/`continue` inside function (should error)
- [ ] `break`/`continue` inside method (should error)
- [ ] `break`/`continue` at top level (graceful in v0.0.15+, previously crashed)
- [ ] `break`/`continue` in class body outside loop (graceful in v0.0.15+)
- [ ] `return` at top level (graceful in v0.0.15+)
- [ ] `return` in class body outside function (graceful in v0.0.15+)

## Objects
- [ ] `new` with no fields
- [ ] `new` with field initializers
- [ ] Method calling sibling method
- [ ] Method modifying fields
- [ ] Two classes with same method name
- [ ] Object field assignment
- [ ] Object as function argument

## Const
- [ ] Direct reassignment
- [ ] push on const array
- [ ] pop on const array
- [ ] Index assign on const array
- [ ] Dot assign on const object
- [ ] ++/-- on const variable

## Imports
- [ ] Import with function calls
- [ ] Import with class instantiation
- [ ] Circular import
- [ ] Missing import file
- [ ] Same-named methods in imported and local classes

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
- [ ] `fs.readFile` on existing file
- [ ] `fs.readFile` on missing file
- [ ] `fs.writeFile` and read back
