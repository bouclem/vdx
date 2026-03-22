import DocPage from "@/components/DocPage";

export default function ScopingDoc() {
  return (
    <DocPage title="Scoping Rules">
      <p>
        VDX uses block scoping. Variables declared inside <code className="text-[#A78BFA]">{"{ }"}</code> blocks
        are local to that block and are destroyed when the block ends.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">How scopes work</h2>
      <p>Each of these creates a new scope:</p>
      <ul className="list-disc list-inside space-y-2 text-sm">
        <li>Class body</li>
        <li>Function body</li>
        <li><code className="text-[#A78BFA]">if</code> / <code className="text-[#A78BFA]">elif</code> / <code className="text-[#A78BFA]">else</code> blocks</li>
        <li><code className="text-[#A78BFA]">while</code> loop body (new scope per iteration)</li>
      </ul>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Inner scopes can read outer variables</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    let x = 10;

    if (x > 5) {
        print(x);    // 10 — reads from class scope
    }
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Inner variables don't leak out</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    if (1) {
        let secret = "hidden";
        print(secret);    // works: hidden
    }

    // print(secret);    // ERROR: undefined variable 'secret'
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Reassignment crosses scopes</h2>
      <p>
        You can reassign an outer variable from an inner scope:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    let count = 0;

    fn increment() {
        count = count + 1;
    }

    increment();
    increment();
    print(count);    // 2
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">While loop scoping</h2>
      <p>
        Each iteration of a <code className="text-[#A78BFA]">while</code> loop gets a fresh scope.
        Variables declared with <code className="text-[#A78BFA]">let</code> inside the loop body
        are recreated each iteration:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    let i = 3;
    @unsafe while (i > 0) {
        let msg = "iteration";    // fresh each time
        print(msg, i);
        i = i - 1;
    }
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Function parameters</h2>
      <p>
        Function parameters live in the function's scope and do not affect outer variables
        with the same name:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    let x = 100;

    fn show(x) {
        print(x);    // prints the parameter, not 100
    }

    show(42);        // 42
    print(x);        // 100 — unchanged
}`}</code></pre>
      </div>
    </DocPage>
  );
}
