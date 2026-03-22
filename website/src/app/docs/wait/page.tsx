import DocPage from "@/components/DocPage";

export default function WaitDoc() {
  return (
    <DocPage title="wait(ms)">
      <p>
        <code className="text-[#A78BFA]">wait(ms)</code> pauses execution for the given number
        of milliseconds.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Syntax</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>wait(milliseconds);</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Examples</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`wait(1000);    // pause 1 second
wait(500);     // pause 0.5 seconds
wait(3000);    // pause 3 seconds`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Use with loops</h2>
      <p>
        <code className="text-[#A78BFA]">wait()</code> is commonly used inside{" "}
        <a href="/docs/while" className="text-[#A78BFA] hover:underline">while loops</a> to
        satisfy the{" "}
        <a href="/docs/loop-protection" className="text-[#A78BFA] hover:underline">loop protection</a>{" "}
        (2-second minimum per iteration):
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    let i = 3;
    while (i > 0) {
        print("tick:", i);
        i = i - 1;
        wait(2100);    // keeps iteration above 2s
    }
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Rules</h2>
      <ul className="list-disc list-inside space-y-2 text-sm">
        <li>Argument must be an integer (milliseconds)</li>
        <li>Negative values throw a runtime error</li>
        <li><code className="text-[#A78BFA]">wait(0)</code> is valid but does nothing</li>
      </ul>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Errors</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm text-red-400"><code>{`wait("hello");  // [VDX] wait() expects an integer (milliseconds)
wait(-100);     // [VDX] wait() duration cannot be negative`}</code></pre>
      </div>
    </DocPage>
  );
}
