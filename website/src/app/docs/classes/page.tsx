import DocPage from "@/components/DocPage";

export default function ClassesDoc() {
  return (
    <DocPage title="Classes">
      <p>
        Classes are the primary way to organize code in VDX. Starting with v0.0.15,
        classes are <strong>recommended but optional</strong> — you can write top-level
        statements directly. However, classes are still essential for object instantiation
        and for organizing larger programs.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Syntax</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`class ClassName {
    // variables, functions, statements
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Rules</h2>
      <ul className="list-disc list-inside space-y-2 text-sm">
        <li>A <code className="text-[#A78BFA]">.vdx</code> file can contain zero or more classes (v0.0.15+)</li>
        <li>Class names must start with a letter or underscore</li>
        <li>Code outside a class is allowed — top-level statements run in order</li>
        <li>A file can contain multiple classes — they run in order</li>
        <li>If no class is present, a recommendation tip is printed to stderr</li>
      </ul>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Execution order</h2>
      <p>
        Inside a class, VDX does two passes:
      </p>
      <ol className="list-decimal list-inside space-y-2 text-sm">
        <li>Register all functions (so you can call them before they appear in code)</li>
        <li>Execute all non-function statements top-to-bottom</li>
      </ol>

      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`class App {
    // This works — greet() is registered in pass 1
    greet();

    fn greet() {
        print("hello");
    }
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Multiple classes</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`class First {
    print("I run first");
}

class Second {
    print("I run second");
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Classes as blueprints</h2>
      <p>
        Classes can also be used as blueprints for object instantiation with{" "}
        <code className="text-[#A78BFA]">new</code>. See{" "}
        <a href="/docs/objects" className="text-[#A78BFA] hover:underline">Objects (new)</a>.
      </p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`class Point {
    let x: int = 0;
    let y: int = 0;
}

class Main {
    let p = new Point();
    p.x = 10;
    print(p.x);   // 10
}`}</code></pre>
      </div>
    </DocPage>
  );
}
