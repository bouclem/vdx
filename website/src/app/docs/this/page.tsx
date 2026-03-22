import DocPage from "@/components/DocPage";

export default function ThisDoc() {
  return (
    <DocPage title="this Keyword">
      <p>
        <code className="text-[#A78BFA]">this</code> accesses variables declared at the
        class scope from anywhere inside the class.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Syntax</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>this.variableName</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Why use this?</h2>
      <p>
        When you are inside a function, local parameters can shadow class-level
        variables. <code className="text-[#A78BFA]">this</code> lets you explicitly reach
        the class scope.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Example</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    let name = "VDX";
    let version = 1;

    print(this.name);       // VDX
    print(this.version);    // 1
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Inside functions</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    let lang = "VDX";

    fn showLang() {
        // 'this.lang' reaches the class-level variable
        print("Language:", this.lang);
    }

    showLang();    // Language: VDX
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">In conditions</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    let mode = "debug";

    if (this.mode == "debug") {
        print("Debug mode is on");
    }
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Rules</h2>
      <ul className="list-disc list-inside space-y-2 text-sm">
        <li><code className="text-[#A78BFA]">this</code> must be followed by <code className="text-[#A78BFA]">.</code> and a field name</li>
        <li>Using <code className="text-[#A78BFA]">this</code> alone is a syntax error</li>
        <li>Accessing an undefined field throws a runtime error</li>
      </ul>
    </DocPage>
  );
}
