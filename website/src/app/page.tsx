export default function Home() {
  return (
    <div className="max-w-5xl mx-auto px-6">
      {/* Hero */}
      <section className="pt-24 pb-20">
        <p className="text-[var(--vdx-muted)] text-sm font-mono mb-4">v0.0.8 — by Voidware</p>
        <h1 className="text-4xl md:text-5xl font-bold text-white mb-5 tracking-tight leading-tight">
          The VDX Programming Language
        </h1>
        <p className="text-lg text-[var(--vdx-muted)] max-w-xl mb-8 leading-relaxed">
          A fast, safe language built for AI and games. Class-based, interpreted,
          with built-in safety and a familiar syntax.
        </p>
        <div className="flex gap-3">
          <a
            href="/download"
            className="bg-white text-black px-5 py-2.5 rounded-md text-sm font-semibold hover:bg-zinc-200 transition-colors"
          >
            Download
          </a>
          <a
            href="/docs"
            className="border border-[var(--vdx-border)] text-white px-5 py-2.5 rounded-md text-sm font-medium hover:border-zinc-500 transition-colors"
          >
            Read the docs
          </a>
        </div>
      </section>

      {/* Code preview */}
      <section className="pb-24">
        <div className="border border-[var(--vdx-border)] rounded-lg overflow-hidden bg-[var(--vdx-surface)]">
          <div className="flex items-center justify-between px-4 py-2.5 border-b border-[var(--vdx-border)]">
            <span className="text-xs text-[var(--vdx-muted)] font-mono">hello.vdx</span>
            <span className="text-xs text-[var(--vdx-muted)]">VDX 0.0.8</span>
          </div>
          <pre className="p-5 text-[13px] leading-relaxed overflow-x-auto text-zinc-300">
            <code>{`class Hello {
    let name: string = "VDX";
    let version: float = 0.8;
    print("Welcome to", this.name);

    fn max(a, b) {
        if (a > b) { return a; }
        else { return b; }
    }

    print("max(3, 7) =", max(3, 7));

    let langs = ["Java", "C++", "Rust"];
    for (lang in langs) {
        print("Inspired by:", lang);
    }
}`}</code>
          </pre>
        </div>
      </section>

      {/* Features */}
      <section className="pb-24">
        <h2 className="text-sm font-mono text-[var(--vdx-muted)] uppercase tracking-wider mb-8">Features</h2>
        <div className="grid md:grid-cols-2 gap-x-16 gap-y-8">
          <div>
            <h3 className="text-white font-medium mb-1.5">Safe by default</h3>
            <p className="text-[var(--vdx-muted)] text-sm leading-relaxed">
              Loop protection prevents infinite loops at runtime. Opt out with <code className="text-[var(--vdx-accent)]">@unsafe</code> when you need control.
            </p>
          </div>
          <div>
            <h3 className="text-white font-medium mb-1.5">C++17 interpreter</h3>
            <p className="text-[var(--vdx-muted)] text-sm leading-relaxed">
              Tree-walking interpreter written in modern C++. Fast startup, minimal footprint.
            </p>
          </div>
          <div>
            <h3 className="text-white font-medium mb-1.5">Type system</h3>
            <p className="text-[var(--vdx-muted)] text-sm leading-relaxed">
              Optional type annotations with runtime checking. Supports int, float, string, bool, arrays, and objects.
            </p>
          </div>
          <div>
            <h3 className="text-white font-medium mb-1.5">Object-oriented</h3>
            <p className="text-[var(--vdx-muted)] text-sm leading-relaxed">
              Classes, object instantiation with <code className="text-[var(--vdx-accent)]">new</code>, dot access, and methods.
            </p>
          </div>
          <div>
            <h3 className="text-white font-medium mb-1.5">Familiar syntax</h3>
            <p className="text-[var(--vdx-muted)] text-sm leading-relaxed">
              If you know Java, C++, or Python, you already know VDX. Minimal learning curve.
            </p>
          </div>
          <div>
            <h3 className="text-white font-medium mb-1.5">One-click install</h3>
            <p className="text-[var(--vdx-muted)] text-sm leading-relaxed">
              Windows MSI installer adds <code className="text-[var(--vdx-accent)]">vdx</code> to your PATH. Run from any terminal.
            </p>
          </div>
        </div>
      </section>
    </div>
  );
}
