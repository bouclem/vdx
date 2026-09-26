import DocPage from "@/components/DocPage";

export default function ImportDoc() {
  return (
    <DocPage title="Import">
      <p>
        The <code className="text-[#A78BFA]">import</code> statement lets you use code from other VDX files. 
        This enables code reuse and modular project organization.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Basic syntax</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`import "filename.vdx";`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Example</h2>
      <p>Create a utils file (<code className="text-[#A78BFA]">utils.vdx</code>):</p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`// Top-level functions are callable directly after import
fn add(a, b) {
    return a + b;
}

// Classes can also be imported — use them via new + dot-call
class Utils {
    fn greet(name) {
        return "Hello, " + name;
    }
}`}</code></pre>
      </div>

      <p>Import and use it in your main file:</p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`import "utils.vdx";

class Main {
    let sum = add(5, 3);
    print(sum);           // 8 — top-level fn is callable directly
    
    let u = new Utils();
    print(u.greet("VDX")); // Hello, VDX — class method via object
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">What gets imported</h2>
      <ul className="list-disc list-inside space-y-2 text-gray-300">
        <li>Top-level functions become callable directly by name</li>
        <li>All class definitions become available for <code className="text-[#A78BFA]">new</code> instantiation — call their methods on an object (<code className="text-[#A78BFA]">u.greet(...)</code>), not as bare functions</li>
        <li>Top-level statements in imported files are not executed</li>
        <li>Errors inside imported files report the imported file's name and lines (v0.1.5+)</li>
      </ul>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Circular imports</h2>
      <p>
        VDX automatically prevents circular imports. If file A imports file B, and file B imports file A, 
        the second import is silently skipped to prevent infinite loops — including a loop
        back through the entry file (v0.1.5+).
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
