import DocPage from "@/components/DocPage";

export default function ImportDoc() {
  return (
    <DocPage title="Import">
      <p>
        The <code className="text-[#A78BFA]">import</code> statement lets you use code from other VDX files. 
        This enables code reuse and modular project organization.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Basic syntax</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`import "filename.vdx";`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Example</h2>
      <p>Create a utils file (<code className="text-[#A78BFA]">utils.vdx</code>):</p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class Utils {
    fn add(a, b) {
        return a + b;
    }
    
    fn greet(name) {
        return "Hello, " + name;
    }
}`}</code></pre>
      </div>

      <p>Import and use it in your main file:</p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`import "utils.vdx";

class Main {
    let sum = add(5, 3);
    print(sum);           // 8
    
    let msg = greet("VDX");
    print(msg);           // Hello, VDX
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">What gets imported</h2>
      <ul className="list-disc list-inside space-y-2 text-gray-300">
        <li>All functions from imported classes become available globally</li>
        <li>All class definitions become available for <code className="text-[#A78BFA]">new</code> instantiation</li>
        <li>Top-level statements in imported files are not executed</li>
      </ul>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Circular imports</h2>
      <p>
        VDX automatically prevents circular imports. If file A imports file B, and file B imports file A, 
        the second import is silently skipped to prevent infinite loops.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">File paths</h2>
      <p>
        Import paths are relative to the importing file. Both relative paths 
        (<code className="text-[#A78BFA]">"./utils.vdx"</code>) and simple filenames 
        (<code className="text-[#A78BFA]">"utils.vdx"</code>) work.
      </p>
    </DocPage>
  );
}
