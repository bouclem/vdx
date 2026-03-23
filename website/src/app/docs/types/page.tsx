import DocPage from "@/components/DocPage";

export default function TypesDoc() {
  return (
    <DocPage title="Types">
      <p>
        VDX supports four value types:{" "}
        <code className="text-[#A78BFA]">int</code>,{" "}
        <code className="text-[#A78BFA]">float</code>,{" "}
        <code className="text-[#A78BFA]">string</code>, and{" "}
        <code className="text-[#A78BFA]">bool</code>. You can optionally annotate
        variables with a type for runtime checking.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Value types</h2>
      <table className="w-full text-sm border border-white/10 rounded-xl overflow-hidden">
        <thead>
          <tr className="bg-[#110827]">
            <th className="text-left p-3 text-white">Type</th>
            <th className="text-left p-3 text-white">Example</th>
            <th className="text-left p-3 text-white">Description</th>
          </tr>
        </thead>
        <tbody>
          <tr className="border-t border-white/10">
            <td className="p-3"><code className="text-[#A78BFA]">int</code></td>
            <td className="p-3"><code className="text-[#A78BFA]">42</code></td>
            <td className="p-3">Whole numbers</td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3"><code className="text-[#A78BFA]">float</code></td>
            <td className="p-3"><code className="text-[#A78BFA]">3.14</code></td>
            <td className="p-3">Decimal numbers</td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3"><code className="text-[#A78BFA]">string</code></td>
            <td className="p-3"><code className="text-[#A78BFA]">&quot;hello&quot;</code></td>
            <td className="p-3">Text</td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3"><code className="text-[#A78BFA]">bool</code></td>
            <td className="p-3"><code className="text-[#A78BFA]">true</code> / <code className="text-[#A78BFA]">false</code></td>
            <td className="p-3">Boolean values</td>
          </tr>
        </tbody>
      </table>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Float literals</h2>
      <p>
        Any number with a decimal point is a <code className="text-[#A78BFA]">float</code>:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let pi = 3.14;
let half = 0.5;
let five = 5.0;    // float, not int
print(pi);         // 3.14`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Boolean literals</h2>
      <p>
        Use <code className="text-[#A78BFA]">true</code> and{" "}
        <code className="text-[#A78BFA]">false</code> directly:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let active = true;
let done = false;
if (active) {
    print("active!");
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Type annotations</h2>
      <p>
        Add a type after the variable name with <code className="text-[#A78BFA]">:</code> to
        enable runtime type checking:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let count: int = 42;
let pi: float = 3.14;
let name: string = "VDX";
let ok: bool = true;`}</code></pre>
      </div>
      <p>
        Type annotations are optional. Without them, variables accept any value.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Type mismatch errors</h2>
      <p>
        If the value doesn&apos;t match the annotation, VDX throws a runtime error:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let x: int = "hello";
// error: Type mismatch: expected 'int', got 'string'`}</code></pre>
      </div>
      <p className="text-sm text-gray-400">
        The <code className="text-[#A78BFA]">float</code> annotation accepts both{" "}
        <code className="text-[#A78BFA]">float</code> and{" "}
        <code className="text-[#A78BFA]">int</code> values (widening).
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Mixed arithmetic</h2>
      <p>
        When an <code className="text-[#A78BFA]">int</code> and{" "}
        <code className="text-[#A78BFA]">float</code> are used together in an operation,
        the result is automatically promoted to <code className="text-[#A78BFA]">float</code>:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`print(10 + 3.5);    // 13.5
print(2 * 3.14);    // 6.28
print(7 / 2.0);     // 3.5`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Truthiness</h2>
      <table className="w-full text-sm border border-white/10 rounded-xl overflow-hidden">
        <thead>
          <tr className="bg-[#110827]">
            <th className="text-left p-3 text-white">Type</th>
            <th className="text-left p-3 text-white">Truthy</th>
            <th className="text-left p-3 text-white">Falsy</th>
          </tr>
        </thead>
        <tbody>
          <tr className="border-t border-white/10">
            <td className="p-3">int</td>
            <td className="p-3">!= 0</td>
            <td className="p-3">0</td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3">float</td>
            <td className="p-3">!= 0.0</td>
            <td className="p-3">0.0</td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3">string</td>
            <td className="p-3">non-empty</td>
            <td className="p-3">&quot;&quot;</td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3">bool</td>
            <td className="p-3">true</td>
            <td className="p-3">false</td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3">array</td>
            <td className="p-3">non-empty</td>
            <td className="p-3">[]</td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3">object</td>
            <td className="p-3">always</td>
            <td className="p-3">—</td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3">void</td>
            <td className="p-3">—</td>
            <td className="p-3">always</td>
          </tr>
        </tbody>
      </table>
    </DocPage>
  );
}
