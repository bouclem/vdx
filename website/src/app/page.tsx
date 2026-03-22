export default function Home() {
  return (
    <>
      {/* Hero */}
      <section className="flex flex-col items-center justify-center text-center px-6 py-32">
        <div className="mb-6">
          <svg width="80" height="80" viewBox="0 0 512 512" fill="none" xmlns="http://www.w3.org/2000/svg">
            <rect width="512" height="512" rx="96" fill="#6C2BD9" />
            <path d="M152 128L256 384L360 128" stroke="white" strokeWidth="48" strokeLinecap="round" strokeLinejoin="round" />
            <circle cx="256" cy="416" r="16" fill="#A78BFA" />
          </svg>
        </div>
        <h1 className="text-5xl md:text-7xl font-bold text-white mb-4 tracking-tight">
          VDX
        </h1>
        <p className="text-xl md:text-2xl text-gray-400 max-w-2xl mb-8">
          A fast, safe programming language built for AI and games.
        </p>
        <div className="flex gap-4">
          <a
            href="/download"
            className="bg-[#6C2BD9] hover:bg-[#5B21B6] text-white px-8 py-3 rounded-xl text-lg font-medium transition-colors"
          >
            Download
          </a>
          <a
            href="/docs"
            className="border border-white/20 hover:border-white/40 text-white px-8 py-3 rounded-xl text-lg font-medium transition-colors"
          >
            Documentation
          </a>
        </div>
      </section>

      {/* Code preview */}
      <section className="max-w-3xl mx-auto px-6 pb-24">
        <div className="bg-[#110827] border border-white/10 rounded-2xl overflow-hidden">
          <div className="flex items-center gap-2 px-4 py-3 border-b border-white/10">
            <span className="w-3 h-3 rounded-full bg-red-500/80" />
            <span className="w-3 h-3 rounded-full bg-yellow-500/80" />
            <span className="w-3 h-3 rounded-full bg-green-500/80" />
            <span className="ml-3 text-xs text-gray-500">hello.vdx</span>
          </div>
          <pre className="p-6 text-sm leading-relaxed overflow-x-auto">
            <code>{`class Hello {
    let name = "VDX";
    print("Welcome to", this.name);

    fn max(a, b) {
        if (a > b) { return a; }
        else { return b; }
    }

    print("max(3, 7) =", max(3, 7));
}`}</code>
          </pre>
        </div>
      </section>

      {/* Features */}
      <section className="max-w-5xl mx-auto px-6 pb-24">
        <h2 className="text-3xl font-bold text-white text-center mb-12">Why VDX?</h2>
        <div className="grid md:grid-cols-3 gap-8">
          <div className="bg-[#110827] border border-white/10 rounded-xl p-6">
            <div className="text-2xl mb-3">🛡️</div>
            <h3 className="text-lg font-semibold text-white mb-2">Safe by Default</h3>
            <p className="text-gray-400 text-sm">
              Built-in loop protection prevents infinite loops. Use <code className="text-[#A78BFA]">@unsafe</code> when you need full control.
            </p>
          </div>
          <div className="bg-[#110827] border border-white/10 rounded-xl p-6">
            <div className="text-2xl mb-3">⚡</div>
            <h3 className="text-lg font-semibold text-white mb-2">Fast</h3>
            <p className="text-gray-400 text-sm">
              Compiled with C++17. Designed for performance-critical applications like games and AI.
            </p>
          </div>
          <div className="bg-[#110827] border border-white/10 rounded-xl p-6">
            <div className="text-2xl mb-3">🎮</div>
            <h3 className="text-lg font-semibold text-white mb-2">Built for Games &amp; AI</h3>
            <p className="text-gray-400 text-sm">
              From the ground up, VDX is designed with game development and artificial intelligence in mind.
            </p>
          </div>
        </div>
      </section>
    </>
  );
}
