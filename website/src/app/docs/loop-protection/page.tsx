import DocPage from "@/components/DocPage";

export default function LoopProtectionDoc() {
  return (
    <DocPage title="Loop Protection">
      <p>
        VDX includes built-in loop safety. By default, every{" "}
        <code className="text-[#A78BFA]">while</code> loop is monitored at runtime. If a
        single iteration completes in less than{" "}
        <span className="text-white font-semibold">2000 milliseconds (2 seconds)</span>, the
        loop is immediately halted with an error.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Why?</h2>
      <p>
        Infinite loops (or extremely fast loops) can freeze your program, consume
        all CPU, and make debugging painful. VDX prevents this by default so you
        can write safer code without thinking about it.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">How it works</h2>
      <p>
        Each time a <code className="text-[#A78BFA]">while</code> loop body finishes one
        iteration, VDX checks how long that iteration took. If it was under 2
        seconds, the runtime throws an error:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm text-red-400 whitespace-pre-wrap">{`[VDX] Loop safety: iteration completed in 0ms (< 2000ms minimum).
      This loop may be infinite or too fast.
      Use @unsafe before 'while' to disable this protection:
      @unsafe while (condition) { ... }`}</pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Example: blocked loop</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    let i = 0;
    // This will be BLOCKED — each iteration is instant (< 2s)
    while (i < 100) {
        print(i);
        i = i + 1;
    }
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Example: safe loop with wait</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    let i = 0;
    // This is fine — wait(2100) makes each iteration > 2s
    while (i < 5) {
        print(i);
        i = i + 1;
        wait(2100);
    }
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Bypassing with @unsafe</h2>
      <p>
        If you know what you are doing and need a fast loop, use the{" "}
        <a href="/docs/unsafe" className="text-[#A78BFA] hover:underline">@unsafe</a>{" "}
        annotation. See the{" "}
        <a href="/docs/unsafe" className="text-[#A78BFA] hover:underline">@unsafe documentation</a>{" "}
        for details.
      </p>
    </DocPage>
  );
}
