import DocPage from "@/components/DocPage";

export default function IfDoc() {
  return (
    <DocPage title="if / elif / else">
      <p>
        Conditional branching with <code className="text-[#A78BFA]">if</code>,{" "}
        <code className="text-[#A78BFA]">elif</code>, and{" "}
        <code className="text-[#A78BFA]">else</code>.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Syntax</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`if (condition) {
    // runs if condition is true
} elif (condition2) {
    // runs if condition2 is true
} else {
    // runs if nothing above matched
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Simple if</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let x = 10;
if (x > 5) {
    print("x is big");
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">if / else</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let age = 15;
if (age >= 18) {
    print("adult");
} else {
    print("minor");
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">if / elif / else</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let score = 85;
if (score >= 90) {
    print("A");
} elif (score >= 80) {
    print("B");
} elif (score >= 70) {
    print("C");
} else {
    print("F");
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Multiple elif</h2>
      <p>You can chain as many <code className="text-[#A78BFA]">elif</code> blocks as needed.</p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Truthiness</h2>
      <p>Conditions are evaluated for truthiness:</p>
      <table className="w-full text-sm border border-white/10 rounded-xl overflow-hidden">
        <thead>
          <tr className="bg-[#110827]">
            <th className="text-left p-3 text-white">Type</th>
            <th className="text-left p-3 text-white">Truthy</th>
            <th className="text-left p-3 text-white">Falsy</th>
          </tr>
        </thead>
        <tbody>
          <tr className="border-t border-white/10"><td className="p-3">Integer</td><td className="p-3">Any non-zero</td><td className="p-3"><code>0</code></td></tr>
          <tr className="border-t border-white/10"><td className="p-3">String</td><td className="p-3">Non-empty</td><td className="p-3"><code>""</code></td></tr>
          <tr className="border-t border-white/10"><td className="p-3">Bool</td><td className="p-3"><code>true</code></td><td className="p-3"><code>false</code></td></tr>
          <tr className="border-t border-white/10"><td className="p-3">Void</td><td className="p-3">—</td><td className="p-3">Always falsy</td></tr>
        </tbody>
      </table>
    </DocPage>
  );
}
