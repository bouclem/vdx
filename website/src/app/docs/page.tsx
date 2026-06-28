const docSections = [
  {
    title: "Getting Started",
    items: [
      { name: "Installation", href: "/docs/installation" },
      { name: "Hello World", href: "/docs/hello-world" },
      { name: "Project Structure", href: "/docs/project-structure" },
    ],
  },
  {
    title: "Language Basics",
    items: [
      { name: "Classes", href: "/docs/classes" },
      { name: "Variables (let)", href: "/docs/variables" },
      { name: "Constants (const)", href: "/docs/const" },
      { name: "Types", href: "/docs/types" },
      { name: "Functions (fn)", href: "/docs/functions" },
      { name: "Operators", href: "/docs/operators" },
      { name: "Modulo (%)", href: "/docs/modulo" },
      { name: "Increment/Decrement (++/--)", href: "/docs/increment-decrement" },
      { name: "Arrays", href: "/docs/arrays" },
      { name: "Objects (new)", href: "/docs/objects" },
      { name: "Dictionaries", href: "/docs/dictionaries" },
      { name: "Print", href: "/docs/print" },
      { name: "Import", href: "/docs/import" },
    ],
  },
  {
    title: "Built-in Functions",
    items: [
      { name: "type()", href: "/docs/type" },
      { name: "input()", href: "/docs/input" },
      { name: "len()", href: "/docs/len" },
      { name: "push() / pop()", href: "/docs/array-functions" },
    ],
  },
  {
    title: "Modules",
    items: [
      { name: "Math Module", href: "/docs/math" },
      { name: "Filesystem Module", href: "/docs/fs" },
    ],
  },
  {
    title: "Control Flow",
    items: [
      { name: "if / elif / else", href: "/docs/if-elif-else" },
      { name: "while Loops", href: "/docs/while" },
      { name: "for Loops", href: "/docs/for" },
      { name: "break", href: "/docs/break" },
      { name: "continue", href: "/docs/continue" },
      { name: "wait(ms)", href: "/docs/wait" },
    ],
  },
  {
    title: "Safety",
    items: [
      { name: "Loop Protection", href: "/docs/loop-protection" },
      { name: "@unsafe", href: "/docs/unsafe" },
      { name: "Error Reporting", href: "/docs/error-reporting" },
    ],
  },
  {
    title: "Advanced",
    items: [
      { name: "this Keyword", href: "/docs/this" },
      { name: "Scoping Rules", href: "/docs/scoping" },
      { name: "Return Values", href: "/docs/return" },
    ],
  },
];

export default function DocsPage() {
  return (
    <div className="max-w-5xl mx-auto px-6 py-16">
      <h1 className="text-3xl font-bold text-white mb-2">Documentation</h1>
      <p className="text-[var(--vdx-muted)] mb-12">
        Language reference for VDX v0.0.15.
      </p>

      <div className="grid md:grid-cols-2 gap-x-16 gap-y-10">
        {docSections.map((section) => (
          <div key={section.title}>
            <h2 className="text-xs font-mono text-[var(--vdx-muted)] uppercase tracking-wider mb-3">{section.title}</h2>
            <ul className="space-y-1.5">
              {section.items.map((item) => (
                <li key={item.href}>
                  <a
                    href={item.href}
                    className="text-sm text-zinc-300 hover:text-white transition-colors"
                  >
                    {item.name}
                  </a>
                </li>
              ))}
            </ul>
          </div>
        ))}
      </div>
    </div>
  );
}
