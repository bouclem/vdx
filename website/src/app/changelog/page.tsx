// Mapping of code terms to documentation links
const codeLinks: Record<string, string> = {
  "break": "/docs/break",
  "continue": "/docs/continue",
  "const": "/docs/const",
  "math": "/docs/math",
  "let": "/docs/variables",
  "fn": "/docs/functions",
  "if": "/docs/if-elif-else",
  "elif": "/docs/if-elif-else",
  "else": "/docs/if-elif-else",
  "while": "/docs/while",
  "for": "/docs/for",
  "return": "/docs/return",
  "this": "/docs/this",
  "print": "/docs/print",
  "wait": "/docs/wait",
  "@unsafe": "/docs/unsafe",
  "new": "/docs/objects",
  "true": "/docs/types",
  "false": "/docs/types",
  "int": "/docs/types",
  "float": "/docs/types",
  "string": "/docs/types",
  "bool": "/docs/types",
  "%": "/docs/modulo",
  "++": "/docs/increment-decrement",
  "--": "/docs/increment-decrement",
};

// Replace code ticks with linked versions
function linkifyCode(text: string): string {
  return text.replace(/`([^`]+)`/g, (match, code) => {
    const link = codeLinks[code];
    if (link) {
      return `<a href="${link}" class="text-[var(--vdx-accent)] text-[13px] hover:underline">${code}</a>`;
    }
    return `<code class="text-[var(--vdx-accent)] text-[13px]">${code}</code>`;
  });
}

const changelog = [
  {
    version: "0.0.12",
    date: "2026-06-15",
    changes: [
      "Added dictionary/map type: `let user = {\"name\": \"Alice\", \"age\": 30};`",
      "Dictionary access: `user[\"name\"]` returns the value for a key",
      "Dictionary assignment: `user[\"city\"] = \"Paris\";` adds or updates keys",
      "Dictionary length: `len(user)` returns number of key-value pairs",
      "Added `fs` module with file I/O: `fs.readFile(path)` and `fs.writeFile(path, content)`",
      "Added array type annotations: `let nums: int[] = [1, 2, 3];`",
      "Added `dict` type annotation: `let user: dict = {\"name\": \"Bob\"};`",
    ],
  },
  {
    version: "0.0.11",
    date: "2026-06-15",
    changes: [
      "Added `import` statement: import other VDX files with `import \"filename.vdx\";`",
      "Imported files provide access to their classes and functions",
      "Circular import protection prevents infinite loops",
      "Added `type(value)` built-in: returns type name as string (\"int\", \"float\", \"string\", \"bool\", \"array\", \"object\", \"void\")",
      "Added `input()` built-in: read user input from stdin",
      "Added `input(prompt)` variant: print prompt then read input",
      "Added `pop(arr)` built-in: remove and return last element from array",
      "Extended `len(obj)`: now works with objects, returning number of fields",
      "`len()` now supports arrays, strings, and objects",
    ],
  },
  {
    version: "0.0.10",
    date: "2026-06-08",
    changes: [
      "Added modulo operator `%` for game calculations (e.g., `x % 2`, `playerIndex % teamSize`)",
      "Added increment `++` and decrement `--` operators",
      "Prefix form: `++x` and `--x` (returns new value)",
      "Postfix form: `x++` and `x--` (returns old value)",
      "MSI installer now properly adds vdx to system PATH",
    ],
  },
  {
    version: "0.0.9",
    date: "2026-06-08",
    changes: [
      "Added `break` statement for exiting loops early",
      "Added `continue` statement for skipping to next loop iteration",
      "Added `const` keyword for declaring immutable constants",
      "Constants support type annotations: `const PI: float = 3.14;`",
      "Added `math` module with 12+ functions: `sqrt`, `pow`, `abs`, `sin`, `cos`, `tan`, `floor`, `ceil`, `round`, `min`, `max`, `random`, `pi`",
      "Interpreter tracks const variables to prevent reassignment",
      "Loop control statements work with `while`, `for`, and `for-in` loops",
      "Break/continue exceptions properly propagate through function calls",
    ],
  },
  {
    version: "0.0.8",
    date: "2026-03-23",
    changes: [
      "Added `float` type: float literals with decimal point (e.g., `3.14`, `5.0`)",
      "Added `true` / `false` boolean literals",
      "Added optional type annotations on `let`: `let x: int = 5;`, `let pi: float = 3.14;`",
      "Runtime type checking: annotated variables are validated at assignment",
      "Mixed int/float arithmetic: operations auto-promote to float when either operand is float",
      "Added `new` keyword for object instantiation: `let obj = new ClassName();`",
      "Dot field access: `obj.field` reads a field from an object",
      "Dot field assignment: `obj.field = value;` sets a field on an object",
      "Dot method calls: `obj.method(args)` calls a method with access to object fields",
      "Objects print as `<ClassName object>`",
      "Added C-style `for` loop: `for (let i = 0; i < n; i = i + 1) { ... }`",
      "Added `for-in` loop over arrays: `for (item in arr) { ... }`",
      "`@unsafe` now also works with `for` loops",
      "Float truthiness: `0.0` is falsy, non-zero is truthy",
      "Object truthiness: objects are truthy",
    ],
  },
  {
    version: "0.0.7",
    date: "2026-03-22",
    changes: [
      "Added arrays / lists: `let arr = [1, 2, 3];`",
      "Array index access: `arr[0]`",
      "Array index assignment: `arr[0] = 5;`",
      "String index access: `str[0]` returns single character",
      "Built-in `len()`: returns length of arrays and strings",
      "Built-in `push(arr, value)`: appends a value to an array",
      "Array printing: `print(arr)` outputs `[1, 2, 3]`",
      "Array truthiness: non-empty arrays are truthy, empty arrays are falsy",
      "Improved error reporting: errors now display file name, line number, and surrounding source lines",
    ],
  },
  {
    version: "0.0.6",
    date: "2026-03-22",
    changes: [
      "Added loop safety protection: `while` loops that iterate faster than 2 seconds are blocked",
      "Added `@unsafe` annotation: place before `while` to bypass loop speed protection",
      "Added SVG icon (`assets/icon.svg`)",
      "Added Windows MSI installer support via CMake CPack + WiX",
      "Installer adds `vdx` to system PATH",
      "Added `.vdx` file association in installer",
      "Added LICENSE.txt (MIT)",
      "Website v0.1.0: main page, download page, documentation pages",
    ],
  },
  {
    version: "0.0.5",
    date: "2026-03-22",
    changes: [
      "Added `while` loop: `while (condition) { body }`",
      "Added block scoping: variables declared inside `{ }` blocks are local",
      "Added `wait(ms)` statement to pause execution",
      "Added variable reassignment: `name = expr;`",
    ],
  },
  {
    version: "0.0.4",
    date: "2026-03-22",
    changes: [
      "Added `this` keyword for accessing class-scope variables",
      "Added `if` / `elif` / `else` control flow",
      "Inline expression evaluation in `print()`",
      "Truthiness rules: int != 0, non-empty string, bool value, void = false",
    ],
  },
  {
    version: "0.0.3",
    date: "2026-03-22",
    changes: [
      "Added `fn` keyword for function declarations with parameters",
      "Added `return` statement for function return values",
      "Function calls as expressions",
      "Operators: `+`, `-`, `*`, `/`, `==`, `!=`, `<`, `>`, `<=`, `>=`",
      "String concatenation with `+`",
      "Operator precedence and parenthesized expressions",
      "Division by zero runtime error",
      "Two-pass class execution (register functions first, then run statements)",
    ],
  },
  {
    version: "0.0.2",
    date: "2026-03-22",
    changes: [
      "Added `let` keyword for variable declarations",
      "Variables support string and integer values",
      "Variables can be used in `print()` arguments",
      "Added expression system (string literals, integer literals, identifiers)",
    ],
  },
  {
    version: "0.0.1",
    date: "2026-03-22",
    changes: [
      "Initial release",
      "Lexer with support for strings, integers, identifiers, keywords, symbols",
      "Parser for `class` declarations and `print()` statements",
      "Tree-walking interpreter",
      "`class` keyword: declare named classes with a body",
      "`print()`: output string literals to stdout",
      "Line comments (`//`)",
    ],
  },
];

export default function ChangelogPage() {
  return (
    <div className="max-w-5xl mx-auto px-6 py-16">
      <h1 className="text-3xl font-bold text-white mb-2">Changelog</h1>
      <p className="text-[var(--vdx-muted)] mb-12">
        All notable changes to VDX, by version.
      </p>

      <div className="space-y-16">
        {changelog.map((release) => (
          <section key={release.version} id={`v${release.version.replace(/\./g, "")}`}>
            <div className="flex items-baseline gap-4 mb-4">
              <h2 className="text-lg font-mono font-medium text-white">v{release.version}</h2>
              <span className="text-sm text-[var(--vdx-muted)]">{release.date}</span>
            </div>
            <ul className="space-y-1.5 text-sm text-zinc-300">
              {release.changes.map((change, i) => (
                <li key={i} className="flex gap-2 leading-relaxed">
                  <span className="text-[var(--vdx-border)] shrink-0">—</span>
                  <span dangerouslySetInnerHTML={{
                    __html: linkifyCode(change)
                  }} />
                </li>
              ))}
            </ul>
          </section>
        ))}
      </div>
    </div>
  );
}
