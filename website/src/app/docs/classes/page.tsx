import DocPage from "@/components/DocPage";

export default function ClassesDoc() {
  return (
    <DocPage title="Classes">
      <p>
        Every VDX program is organized into classes. A class is the top-level
        container for all code — variables, functions, and statements.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Syntax</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class ClassName {
    // variables, functions, statements
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Rules</h2>
      <ul className="list-disc list-inside space-y-2 text-sm">
        <li>Every <code className="text-[#A78BFA]">.vdx</code> file must contain at least one class</li>
        <li>Class names must start with a letter or underscore</li>
        <li>Code outside a class is not allowed</li>
        <li>A file can contain multiple classes — they run in order</li>
      </ul>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Execution order</h2>
      <p>
        Inside a class, VDX does two passes:
      </p>
      <ol className="list-decimal list-inside space-y-2 text-sm">
        <li>Register all functions (so you can call them before they appear in code)</li>
        <li>Execute all non-function statements top-to-bottom</li>
      </ol>

      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    // This works — greet() is registered in pass 1
    greet();

    fn greet() {
        print("hello");
    }
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Multiple classes</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
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
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
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
