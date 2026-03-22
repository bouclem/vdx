import DocPage from "@/components/DocPage";

export default function UnsafeDoc() {
  return (
    <DocPage title="@unsafe">
      <div className="bg-red-900/20 border border-red-500/30 rounded-xl p-5 my-4">
        <p className="text-red-300 font-semibold mb-2">⚠️ Warning</p>
        <p className="text-red-200 text-sm">
          <code>@unsafe</code> disables safety protections. Only use it when you
          fully understand the consequences. An unprotected loop can freeze your
          program indefinitely.
        </p>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">What is @unsafe?</h2>
      <p>
        <code className="text-[#A78BFA]">@unsafe</code> is an annotation you place directly
        before a <code className="text-[#A78BFA]">while</code> loop to disable the built-in{" "}
        <a href="/docs/loop-protection" className="text-[#A78BFA] hover:underline">loop protection</a>.
      </p>
      <p>
        Without <code className="text-[#A78BFA]">@unsafe</code>, any loop iteration that
        completes in under 2 seconds is blocked. With{" "}
        <code className="text-[#A78BFA]">@unsafe</code>, the loop runs with no speed checks.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Syntax</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`@unsafe while (condition) {
    // loop body — no speed protection
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Example</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    fn fastCount(n) {
        @unsafe while (n > 0) {
            print("count:", n);
            n = n - 1;
        }
    }

    fastCount(1000);
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">When to use @unsafe</h2>
      <ul className="list-disc list-inside space-y-2 text-sm">
        <li>Tight computational loops (math, sorting, processing)</li>
        <li>Game loops that run at high frame rates</li>
        <li>Benchmarking or performance testing</li>
        <li>Any loop where you are certain it will terminate</li>
      </ul>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">When NOT to use @unsafe</h2>
      <ul className="list-disc list-inside space-y-2 text-sm">
        <li>When you are unsure if the loop will terminate</li>
        <li>In production code where stability matters more than speed</li>
        <li>When learning VDX — let the safety system help you</li>
      </ul>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">What @unsafe disables</h2>
      <table className="w-full text-sm border border-white/10 rounded-xl overflow-hidden">
        <thead>
          <tr className="bg-[#110827]">
            <th className="text-left p-3 text-white">Protection</th>
            <th className="text-left p-3 text-white">Normal while</th>
            <th className="text-left p-3 text-white">@unsafe while</th>
          </tr>
        </thead>
        <tbody>
          <tr className="border-t border-white/10">
            <td className="p-3">Iteration speed check (&lt; 2s)</td>
            <td className="p-3 text-green-400">✓ Active</td>
            <td className="p-3 text-red-400">✗ Disabled</td>
          </tr>
        </tbody>
      </table>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Rules</h2>
      <ul className="list-disc list-inside space-y-2 text-sm">
        <li>
          <code className="text-[#A78BFA]">@unsafe</code> can only be placed directly before{" "}
          <code className="text-[#A78BFA]">while</code>
        </li>
        <li>It cannot be used on other statements (if, fn, let, etc.)</li>
        <li>
          Using <code className="text-[#A78BFA]">@unsafe</code> before anything other than{" "}
          <code className="text-[#A78BFA]">while</code> is a compile error
        </li>
      </ul>
    </DocPage>
  );
}
