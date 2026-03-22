import DocPage from "@/components/DocPage";

export default function HelloWorldDoc() {
  return (
    <DocPage title="Hello World">
      <p>
        Your first VDX program. Every VDX program starts with a{" "}
        <code className="text-[#A78BFA]">class</code> declaration.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">1. Create a file</h2>
      <p>
        Create a file called <code className="text-[#A78BFA]">hello.vdx</code>:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class Hello {
    print("Hello, world!");
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">2. Run it</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>vdx hello.vdx</code></pre>
      </div>
      <p>Output:</p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm text-green-400"><code>Hello, world!</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">3. What happened?</h2>
      <ul className="list-disc list-inside space-y-2 text-sm">
        <li>
          <code className="text-[#A78BFA]">class Hello {"{"} ... {"}"}</code> — declares a class
          named <code className="text-[#A78BFA]">Hello</code>. All code lives inside a class.
        </li>
        <li>
          <code className="text-[#A78BFA]">print("Hello, world!");</code> — prints text to the
          terminal. Every statement ends with <code className="text-[#A78BFA]">;</code>
        </li>
      </ul>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">A bigger example</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class Hello {
    let name = "VDX";
    print("Welcome to", this.name);

    print(1 + 1);
    print(10 * 3 + 2);

    fn greet(who) {
        print("Hello,", who);
    }

    greet("developer");
}`}</code></pre>
      </div>
      <p>Output:</p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm text-green-400"><code>{`Welcome to VDX
2
32
Hello, developer`}</code></pre>
      </div>
    </DocPage>
  );
}
