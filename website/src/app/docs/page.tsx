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
      { name: "Types", href: "/docs/types" },
      { name: "Functions (fn)", href: "/docs/functions" },
      { name: "Operators", href: "/docs/operators" },
      { name: "Arrays", href: "/docs/arrays" },
      { name: "Objects (new)", href: "/docs/objects" },
      { name: "Print", href: "/docs/print" },
    ],
  },
  {
    title: "Control Flow",
    items: [
      { name: "if / elif / else", href: "/docs/if-elif-else" },
      { name: "while Loops", href: "/docs/while" },
      { name: "for Loops", href: "/docs/for" },
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
    <div className="max-w-4xl mx-auto px-6 py-16">
      <h1 className="text-4xl font-bold text-white mb-4">Documentation</h1>
      <p className="text-gray-400 mb-12 text-lg">
        Everything you need to know about VDX.
      </p>

      <div className="grid md:grid-cols-2 gap-8">
        {docSections.map((section) => (
          <div key={section.title} className="bg-[#110827] border border-white/10 rounded-xl p-6">
            <h2 className="text-lg font-semibold text-white mb-4">{section.title}</h2>
            <ul className="space-y-2">
              {section.items.map((item) => (
                <li key={item.href}>
                  <a
                    href={item.href}
                    className="text-gray-400 hover:text-[#A78BFA] transition-colors text-sm"
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
