import DocPage from "@/components/DocPage";

export default function ProjectStructureDoc() {
  return (
    <DocPage title="Project Structure">
      <p>
        VDX programs are simple — each <code className="text-[#A78BFA]">.vdx</code> file contains
        one or more class declarations. There is no special project layout required.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Minimal project</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`my-project/
  main.vdx`}</code></pre>
      </div>
      <p>Run it with:</p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>vdx main.vdx</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">File extension</h2>
      <p>
        All VDX source files use the <code className="text-[#A78BFA]">.vdx</code> extension.
        The VDX interpreter only accepts <code className="text-[#A78BFA]">.vdx</code> files.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Entry point</h2>
      <p>
        VDX does not have a <code className="text-[#A78BFA]">main()</code> function. Instead,
        code inside a class body runs top-to-bottom when the file is executed. Functions are
        registered first, then statements run in order.
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    // Functions are registered first (pass 1)
    fn sayHi() {
        print("hi");
    }

    // Then statements run in order (pass 2)
    sayHi();
    print("done");
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Comments</h2>
      <p>
        Line comments start with <code className="text-[#A78BFA]">//</code>. There are no
        block comments yet.
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`// This is a comment
print("hello"); // inline comment`}</code></pre>
      </div>
    </DocPage>
  );
}
