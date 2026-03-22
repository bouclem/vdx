import DocPage from "@/components/DocPage";

export default function PrintDoc() {
  return (
    <DocPage title="Print">
      <p>
        <code className="text-[#A78BFA]">print()</code> outputs values to the terminal.
        It accepts any number of arguments separated by commas.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Syntax</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`print(expr);
print(expr1, expr2, expr3);`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Basic usage</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`print("hello");         // hello
print(42);              // 42
print("age:", 25);      // age: 25`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Expressions in print</h2>
      <p>
        You can pass any expression — it gets evaluated before printing:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`print(1 + 1);           // 2
print(10 * 3 + 2);     // 32
print(5 == 5);          // true`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Multiple arguments</h2>
      <p>
        Multiple arguments are separated by a space in the output. Each{" "}
        <code className="text-[#A78BFA]">print()</code> call ends with a newline.
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`print("x =", 10, "y =", 20);
// Output: x = 10 y = 20`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Variables and function calls</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let name = "VDX";
print("Welcome to", name);

fn double(n) { return n * 2; }
print("result:", double(5));    // result: 10`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Escape sequences</h2>
      <table className="w-full text-sm border border-white/10 rounded-xl overflow-hidden">
        <thead>
          <tr className="bg-[#110827]">
            <th className="text-left p-3 text-white">Sequence</th>
            <th className="text-left p-3 text-white">Output</th>
          </tr>
        </thead>
        <tbody>
          <tr className="border-t border-white/10"><td className="p-3"><code>\\n</code></td><td className="p-3">Newline</td></tr>
          <tr className="border-t border-white/10"><td className="p-3"><code>\\t</code></td><td className="p-3">Tab</td></tr>
          <tr className="border-t border-white/10"><td className="p-3"><code>\\\\</code></td><td className="p-3">Backslash</td></tr>
          <tr className="border-t border-white/10"><td className="p-3"><code>\\"</code></td><td className="p-3">Double quote</td></tr>
        </tbody>
      </table>
    </DocPage>
  );
}
