import DocPage from "@/components/DocPage";

export default function OperatorsDoc() {
  return (
    <DocPage title="Operators">
      <p>
        VDX supports arithmetic, comparison, and equality operators.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Arithmetic</h2>
      <table className="w-full text-sm border border-white/10 rounded-xl overflow-hidden">
        <thead>
          <tr className="bg-[#110827]">
            <th className="text-left p-3 text-white">Operator</th>
            <th className="text-left p-3 text-white">Description</th>
            <th className="text-left p-3 text-white">Example</th>
          </tr>
        </thead>
        <tbody>
          <tr className="border-t border-white/10"><td className="p-3"><code>+</code></td><td className="p-3">Addition</td><td className="p-3"><code>3 + 2</code> → <code>5</code></td></tr>
          <tr className="border-t border-white/10"><td className="p-3"><code>-</code></td><td className="p-3">Subtraction</td><td className="p-3"><code>10 - 4</code> → <code>6</code></td></tr>
          <tr className="border-t border-white/10"><td className="p-3"><code>*</code></td><td className="p-3">Multiplication</td><td className="p-3"><code>3 * 4</code> → <code>12</code></td></tr>
          <tr className="border-t border-white/10"><td className="p-3"><code>/</code></td><td className="p-3">Division (integer)</td><td className="p-3"><code>7 / 2</code> → <code>3</code></td></tr>
        </tbody>
      </table>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Comparison</h2>
      <table className="w-full text-sm border border-white/10 rounded-xl overflow-hidden">
        <thead>
          <tr className="bg-[#110827]">
            <th className="text-left p-3 text-white">Operator</th>
            <th className="text-left p-3 text-white">Description</th>
            <th className="text-left p-3 text-white">Example</th>
          </tr>
        </thead>
        <tbody>
          <tr className="border-t border-white/10"><td className="p-3"><code>&lt;</code></td><td className="p-3">Less than</td><td className="p-3"><code>3 &lt; 5</code> → <code>true</code></td></tr>
          <tr className="border-t border-white/10"><td className="p-3"><code>&gt;</code></td><td className="p-3">Greater than</td><td className="p-3"><code>5 &gt; 3</code> → <code>true</code></td></tr>
          <tr className="border-t border-white/10"><td className="p-3"><code>&lt;=</code></td><td className="p-3">Less or equal</td><td className="p-3"><code>3 &lt;= 3</code> → <code>true</code></td></tr>
          <tr className="border-t border-white/10"><td className="p-3"><code>&gt;=</code></td><td className="p-3">Greater or equal</td><td className="p-3"><code>5 &gt;= 3</code> → <code>true</code></td></tr>
        </tbody>
      </table>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Equality</h2>
      <table className="w-full text-sm border border-white/10 rounded-xl overflow-hidden">
        <thead>
          <tr className="bg-[#110827]">
            <th className="text-left p-3 text-white">Operator</th>
            <th className="text-left p-3 text-white">Description</th>
            <th className="text-left p-3 text-white">Example</th>
          </tr>
        </thead>
        <tbody>
          <tr className="border-t border-white/10"><td className="p-3"><code>==</code></td><td className="p-3">Equal</td><td className="p-3"><code>5 == 5</code> → <code>true</code></td></tr>
          <tr className="border-t border-white/10"><td className="p-3"><code>!=</code></td><td className="p-3">Not equal</td><td className="p-3"><code>5 != 3</code> → <code>true</code></td></tr>
        </tbody>
      </table>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">String concatenation</h2>
      <p>
        The <code className="text-[#A78BFA]">+</code> operator also concatenates strings:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let greeting = "Hello" + " " + "world";
print(greeting);    // Hello world`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Precedence</h2>
      <p>From highest to lowest:</p>
      <ol className="list-decimal list-inside space-y-1 text-sm">
        <li><code className="text-[#A78BFA]">*</code> <code className="text-[#A78BFA]">/</code> — multiplication, division</li>
        <li><code className="text-[#A78BFA]">+</code> <code className="text-[#A78BFA]">-</code> — addition, subtraction</li>
        <li><code className="text-[#A78BFA]">&lt;</code> <code className="text-[#A78BFA]">&gt;</code> <code className="text-[#A78BFA]">&lt;=</code> <code className="text-[#A78BFA]">&gt;=</code> — comparison</li>
        <li><code className="text-[#A78BFA]">==</code> <code className="text-[#A78BFA]">!=</code> — equality</li>
      </ol>
      <p className="text-sm mt-2">Use parentheses <code className="text-[#A78BFA]">()</code> to override precedence.</p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Division by zero</h2>
      <p>
        Dividing by zero throws a runtime error:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm text-red-400"><code>[VDX] Division by zero</code></pre>
      </div>
    </DocPage>
  );
}
