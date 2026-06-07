const changelog = [
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
      <p className="text-[--vdx-muted] mb-12">
        All notable changes to VDX, by version.
      </p>

      <div className="space-y-16">
        {changelog.map((release) => (
          <section key={release.version} id={`v${release.version.replace(/\./g, "")}`}>
            <div className="flex items-baseline gap-4 mb-4">
              <h2 className="text-lg font-mono font-medium text-white">v{release.version}</h2>
              <span className="text-sm text-[--vdx-muted]">{release.date}</span>
            </div>
            <ul className="space-y-1.5 text-sm text-zinc-300">
              {release.changes.map((change, i) => (
                <li key={i} className="flex gap-2 leading-relaxed">
                  <span className="text-[--vdx-border] shrink-0">—</span>
                  <span dangerouslySetInnerHTML={{
                    __html: change.replace(/`([^`]+)`/g, '<code class="text-[--vdx-accent] text-[13px]">$1</code>')
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
