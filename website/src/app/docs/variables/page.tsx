import DocPage from "@/components/DocPage";

export default function VariablesDoc() {
  return (
    <DocPage title="Variables (let)">
      <p>
        Variables are declared with <code className="text-[#A78BFA]">let</code> and can hold
        strings, integers, floats, booleans, arrays, or objects.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Declaration</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let name = "VDX";
let age = 1;
let result = 10 + 5;`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Reassignment</h2>
      <p>
        After declaring a variable with <code className="text-[#A78BFA]">let</code>, you can
        reassign it without <code className="text-[#A78BFA]">let</code>:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let x = 10;
x = 20;        // reassign
print(x);      // 20`}</code></pre>
      </div>
      <p className="text-sm text-yellow-300/80">
        ⚠️ You cannot reassign a variable that was never declared with{" "}
        <code className="text-[#A78BFA]">let</code>. This will throw a runtime error.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Types</h2>
      <table className="w-full text-sm border border-white/10 rounded-xl overflow-hidden">
        <thead>
          <tr className="bg-[#110827]">
            <th className="text-left p-3 text-white">Type</th>
            <th className="text-left p-3 text-white">Example</th>
          </tr>
        </thead>
        <tbody>
          <tr className="border-t border-white/10">
            <td className="p-3">String</td>
            <td className="p-3"><code className="text-[#A78BFA]">"hello"</code></td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3">Integer</td>
            <td className="p-3"><code className="text-[#A78BFA]">42</code></td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3">Float</td>
            <td className="p-3"><code className="text-[#A78BFA]">3.14</code></td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3">Boolean</td>
            <td className="p-3"><code className="text-[#A78BFA]">true</code> / <code className="text-[#A78BFA]">false</code></td>
          </tr>
        </tbody>
      </table>
      <p className="text-sm text-gray-400 mt-3">
        You can add optional type annotations for runtime checking. See{" "}
        <a href="/docs/types" className="text-[#A78BFA] hover:underline">Types</a>.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Using variables in print</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let lang = "VDX";
print("Language:", lang);    // Language: VDX`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Class-scope access with this</h2>
      <p>
        Variables declared at the class level can be accessed with{" "}
        <code className="text-[#A78BFA]">this.name</code>. See{" "}
        <a href="/docs/this" className="text-[#A78BFA] hover:underline">this keyword</a>.
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    let version = 1;
    print(this.version);   // 1
}`}</code></pre>
      </div>
    </DocPage>
  );
}
