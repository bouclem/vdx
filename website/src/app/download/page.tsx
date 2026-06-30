const GITHUB_REPO = "bouclem/vdx";

type VersionStatus = "current" | "old" | "deprecated";

function getDownloadUrl(version: string) {
  return `https://github.com/${GITHUB_REPO}/releases/download/v${version}/vdx-${version}-win64.msi`;
}

const statusLabel: Record<VersionStatus, string> = {
  current: "current",
  old: "old",
  deprecated: "deprecated",
};

const statusClass: Record<VersionStatus, string> = {
  current: "text-emerald-400",
  old: "text-[var(--vdx-muted)]",
  deprecated: "text-red-400",
};

const versions: {
  version: string;
  date: string;
  status: VersionStatus;
  notes: string[];
}[] = [
  {
    version: "0.1.2",
    date: "2026-06-30",
    status: "current",
    notes: [
      "Fixed bare method calls losing field mutations on break/continue",
      "Fixed graph.bar() drawing bars from vMin instead of zero line for mixed values",
      "Fixed Value::toString not escaping strings in arrays/dicts",
      "Fixed graph escXml not escaping control characters",
      "Fixed graph PlotState shared across threads (now thread_local)",
      "Fixed lexer not handling \\r and \\0 escape sequences",
      "Fixed math.floor/ceil/round UB on large float-to-int cast",
      "Fixed graph.show() TOCTOU race with tmpnam_s",
    ],
  },
  {
    version: "0.1.1",
    date: "2026-06-30",
    status: "old",
    notes: [
      "Fixed for-in iterating over array copy instead of live array",
      "Fixed method calls losing field mutations on break/continue",
      "Fixed graph.bar() with all-negative values",
      "Fixed integer overflow in ++/-- operators",
      "Fixed empty dict {} parse error",
    ],
  },
  {
    version: "0.1.0",
    date: "2026-06-29",
    status: "old",
    notes: [
      "this system overhaul — this now returns the actual object",
      "Fixed this.field assignment and this.method() parse errors",
      "Fixed method-to-method calls losing object context",
      "Fixed new re-executing side-effect statements on every instantiation",
      "Fixed nested imports not being processed recursively",
      "Fixed math.random(min, max) rejecting min == max",
      "C++ quality: noexcept and const correctness improvements",
      "New graph module: SVG plots (scatter, line, bar, histogram)",
    ],
  },
  {
    version: "0.0.15",
    date: "2026-06-28",
    status: "old",
    notes: [
      "class{} wrapper is no longer mandatory — top-level statements allowed",
      "Top-level function declarations supported at file scope",
      "Top-level functions importable from other files",
      "Recommendation warning when no class declaration is present",
      "Fixed math.round() returning FLOAT instead of INT",
      "Fixed math.random(max) exclusive vs inclusive inconsistency",
      "Fixed return/break/continue crash at top level, in class body, and during new",
      "Fixed scope leak in execClass and execNew on exception",
    ],
  },
  {
    version: "0.0.14",
    date: "2026-06-28",
    status: "old",
    notes: [
      "Fixed scope leak on return inside if/while/for/for-in loops",
      "Fixed math module never being registered at startup",
      "Fixed module function dispatch for math.sqrt() etc.",
      "Module constants like math.pi now work",
      "Fixed std::stoi/std::stod uncaught out_of_range on large literals",
      "Fixed UB: isdigit/isalpha/isalnum now receive unsigned char cast",
      "Fixed const variables modifiable via push(), pop(), arr[i]=, obj.f=, ++/--",
      "Fixed break/continue escaping method call boundaries",
      "Fixed duplicate function names across classes (namespaced as ClassName::funcName)",
      "Fixed math.floor() and math.ceil() returning FLOAT instead of INT",
      "Fixed duplicate class names silently overwriting each other",
      "Added unary minus/plus operator support",
      "Fixed static module registration breaking multiple Interpreter instances",
      "Fixed math functions silently accepting non-numeric arguments",
    ],
  },
  {
    version: "0.0.13",
    date: "2026-06-27",
    status: "old",
    notes: [
      "Fixed scope corruption on continue in while/for loops",
      "Fixed inverted loop safety check (now flags >2s, not <2s)",
      "Fixed break/continue escaping function call boundaries",
      "Fixed import crash on missing files (clean error instead)",
      "Fixed signed integer overflow UB (now throws runtime error)",
      "Fixed duplicate function names across classes silently overwriting",
      "Fixed new capturing temporary variables as object fields",
      "Added func as alias for fn keyword",
      "Added ++/-- syntax in for-loop update",
      "Replaced std::rand() with std::mt19937 for better randomness",
      "Added compiler warnings, portable includes, and code cleanup",
    ],
  },
  {
    version: "0.0.12",
    date: "2026-06-15",
    status: "old",
    notes: [
      "Dictionary/map type: {\"key\": value}",
      "fs.readFile() and fs.writeFile() for file I/O",
      "Array type annotations: int[], string[]",
      "dict type annotation support",
    ],
  },
  {
    version: "0.0.11",
    date: "2026-06-15",
    status: "old",
    notes: [
      "import statement to import other VDX files",
      "type() function to get value type as string",
      "input() function to read user input from stdin",
      "pop() function to remove and return last array element",
      "len() now works with objects (returns field count)",
    ],
  },
  {
    version: "0.0.10",
    date: "2026-06-08",
    status: "old",
    notes: [
      "Modulo operator `%` for game calculations",
      "Increment `++` and decrement `--` operators (prefix and postfix)",
      "MSI installer now properly adds vdx to PATH",
    ],
  },
  {
    version: "0.0.9",
    date: "2026-06-08",
    status: "old",
    notes: [
      "break and continue statements for loop control",
      "const keyword for immutable constants",
      "math module with 12+ functions (sqrt, pow, sin, cos, etc.)",
      "Constants support type annotations",
    ],
  },
  {
    version: "0.0.8",
    date: "2026-03-23",
    status: "old",
    notes: [
      "Types: float literals, true/false booleans, optional type annotations",
      "Object instantiation with new, dot access, and methods",
      "C-style for loop and for-in over arrays",
      "Mixed int/float arithmetic with auto-promotion",
    ],
  },
  {
    version: "0.0.7",
    date: "2026-03-22",
    status: "old",
    notes: [
      "Arrays with index access, assignment, len(), push()",
      "Improved error reporting with source context",
      "String indexing",
    ],
  },
  {
    version: "0.0.6",
    date: "2026-03-22",
    status: "old",
    notes: [
      "Loop safety protection",
      "@unsafe annotation",
      "Windows MSI installer with PATH integration",
    ],
  },
];

export default function DownloadPage() {
  const current = versions.find((v) => v.status === "current") ?? null;

  return (
    <div className="max-w-5xl mx-auto px-6 py-16">
      <h1 className="text-3xl font-bold text-white mb-2">Download</h1>
      <p className="text-[var(--vdx-muted)] mb-12">
        Install VDX on Windows. After installation, run{" "}
        <code className="text-[var(--vdx-accent)]">vdx yourfile.vdx</code> from any terminal.
      </p>

      {/* Current release */}
      {current !== null && (
        <section className="mb-16">
          <div className="border border-[var(--vdx-border)] rounded-lg p-6 bg-[var(--vdx-surface)]">
            <div className="flex items-baseline justify-between mb-4">
              <div className="flex items-baseline gap-3">
                <span className="text-xl font-mono font-medium text-white">v{current.version}</span>
                <span className="text-xs font-mono text-emerald-400">current</span>
              </div>
              <span className="text-sm text-[var(--vdx-muted)]">{current.date}</span>
            </div>
            <ul className="text-sm text-[var(--vdx-muted)] mb-6 space-y-1">
              {current.notes.map((note, i) => (
                <li key={i} className="flex gap-2">
                  <span className="text-[var(--vdx-border)]">—</span>
                  {note}
                </li>
              ))}
            </ul>
            <a
              href={getDownloadUrl(current.version)}
              className="inline-flex items-center gap-2 bg-white text-black px-4 py-2.5 rounded-md text-sm font-semibold hover:bg-zinc-200 transition-colors"
            >
              <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
                <path d="M21 15v4a2 2 0 01-2 2H5a2 2 0 01-2-2v-4" />
                <polyline points="7 10 12 15 17 10" />
                <line x1="12" y1="15" x2="12" y2="3" />
              </svg>
              Windows Installer (.msi)
            </a>
            <p className="text-xs text-[var(--vdx-muted)] mt-4 leading-relaxed">
              <strong className="text-zinc-400">Windows SmartScreen warning?</strong> The installer
              is not digitally signed, so Windows may show a "protected your PC" warning. Click{" "}
              <span className="text-zinc-400">More info</span> &rarr;{" "}
              <span className="text-zinc-400">Run anyway</span>. The installer is safe — it was built
              from the open-source code in the{" "}
              <a href="https://github.com/bouclem/vdx" className="text-[var(--vdx-accent)] hover:text-white">
                GitHub repo
              </a>.
            </p>
          </div>
        </section>
      )}

      {/* Version history */}
      <section>
        <h2 className="text-sm font-mono text-[var(--vdx-muted)] uppercase tracking-wider mb-6">All releases</h2>
        <div className="border border-[var(--vdx-border)] rounded-lg divide-y divide-[var(--vdx-border)]">
          {versions.map((v) => {
            const isDownloadable = v.status !== "deprecated";

            return (
              <div
                key={v.version}
                className={`px-5 py-4 flex items-center justify-between ${
                  v.status === "deprecated" ? "opacity-50" : ""
                }`}
              >
                <div className="flex items-baseline gap-3">
                  <span className={`font-mono text-sm text-white ${v.status === "deprecated" ? "line-through" : ""}`}>
                    v{v.version}
                  </span>
                  <span className={`text-xs font-mono ${statusClass[v.status]}`}>
                    {statusLabel[v.status]}
                  </span>
                  <span className="text-xs text-[var(--vdx-muted)]">{v.date}</span>
                </div>
                {isDownloadable ? (
                  <a
                    href={getDownloadUrl(v.version)}
                    className="text-[var(--vdx-accent)] hover:text-white text-sm font-mono"
                  >
                    .msi
                  </a>
                ) : (
                  <a
                    href={`/changelog#v${v.version.replace(/\./g, "")}`}
                    className="text-[var(--vdx-muted)] hover:text-white text-sm font-mono"
                  >
                    changelog
                  </a>
                )}
              </div>
            );
          })}
        </div>
        <p className="text-xs text-[var(--vdx-muted)] mt-4">
          Deprecated versions link to their changelog entry. Downloads are served from{" "}
          <a href={`https://github.com/${GITHUB_REPO}/releases`} className="text-[var(--vdx-accent)] hover:text-white">
            GitHub Releases
          </a>.
        </p>
      </section>
    </div>
  );
}
