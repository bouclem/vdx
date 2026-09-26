import DocPage from "@/components/DocPage";

export default function LoopProtectionDoc() {
  return (
    <DocPage title="Loop Protection">
      <p>
        VDX includes built-in loop safety. By default, every{" "}
        <code className="text-[#A78BFA]">while</code>,{" "}
        <code className="text-[#A78BFA]">for</code>, and{" "}
        <code className="text-[#A78BFA]">for-in</code> loop is monitored at runtime
        and halted when either limit is hit:
      </p>
      <ul className="list-disc list-inside space-y-2 text-sm">
        <li>
          A single iteration takes more than{" "}
          <span className="text-white font-semibold">2000 ms (2 seconds)</span> of real work
        </li>
        <li>
          The loop runs more than{" "}
          <span className="text-white font-semibold">1,000,000 iterations</span> (v0.1.5+)
        </li>
      </ul>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Why?</h2>
      <p>
        Infinite loops (or extremely fast loops) can freeze your program, consume
        all CPU, and make debugging painful. VDX prevents this by default so you
        can write safer code without thinking about it.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">How it works</h2>
      <p>
        Each time a loop body finishes one iteration, VDX checks how much work that
        iteration did and how many iterations have run. If an iteration took more than
        2 seconds of real work, or the loop passes 1,000,000 iterations, the runtime
        throws an error:
      </p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm text-red-400 whitespace-pre-wrap">{`[VDX] Loop safety: iteration took 2500ms (> 2000ms maximum).
      This loop may be infinite or too slow.
      Use @unsafe before 'while' to disable this protection.

[VDX] Loop safety: while loop exceeded 1000000 iterations.
      This loop may be infinite.
      Use @unsafe before 'while' to disable this protection.`}</pre>
      </div>
      <p>
        Time blocked inside{" "}
        <code className="text-[#A78BFA]">wait()</code> and{" "}
        <code className="text-[#A78BFA]">input()</code> does <em>not</em> count toward the
        2-second iteration budget (v0.1.5+) — only real computation is measured.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Example: infinite loop caught</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`class App {
    // BLOCKED after 1,000,000 iterations — even though each one is instant
    while (true) {
        // forgot to break / update a counter
    }
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Example: safe loops</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`class App {
    // Fine — fast iterations, well under the cap
    let i = 0;
    while (i < 5) {
        print(i);
        i = i + 1;
    }

    // Fine — waiting doesn't count toward the 2s iteration budget (v0.1.5+)
    let n = 0;
    while (n < 5) {
        wait(2500);   // blocked time is excluded
        n = n + 1;
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
