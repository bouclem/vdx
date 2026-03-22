import DocPage from "@/components/DocPage";

export default function WhileDoc() {
  return (
    <DocPage title="while Loops">
      <p>
        <code className="text-[#A78BFA]">while</code> repeats a block of code as long as a
        condition is true.
      </p>

      <div className="bg-yellow-900/20 border border-yellow-500/30 rounded-xl p-5 my-4">
        <p className="text-yellow-300 font-semibold mb-2">⚠️ Loop Protection</p>
        <p className="text-yellow-200 text-sm">
          By default, VDX blocks loops that iterate faster than 2 seconds. See{" "}
          <a href="/docs/loop-protection" className="text-[#A78BFA] hover:underline">Loop Protection</a>{" "}
          and <a href="/docs/unsafe" className="text-[#A78BFA] hover:underline">@unsafe</a>.
        </p>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Syntax</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`while (condition) {
    // body
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Example with wait</h2>
      <p>
        To satisfy the loop protection, use{" "}
        <a href="/docs/wait" className="text-[#A78BFA] hover:underline">wait()</a> to slow
        each iteration to at least 2 seconds:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    let i = 5;
    while (i > 0) {
        print("countdown:", i);
        i = i - 1;
        wait(2100);
    }
    print("done!");
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Fast loops with @unsafe</h2>
      <p>
        If you need a fast loop, use{" "}
        <a href="/docs/unsafe" className="text-[#A78BFA] hover:underline">@unsafe</a>:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    let i = 0;
    @unsafe while (i < 100) {
        print(i);
        i = i + 1;
    }
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Loop with function</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    fn countdown(n) {
        @unsafe while (n > 0) {
            print(n);
            n = n - 1;
        }
    }

    countdown(5);
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Scoping</h2>
      <p>
        Variables declared inside a <code className="text-[#A78BFA]">while</code> block are
        local to that iteration. See{" "}
        <a href="/docs/scoping" className="text-[#A78BFA] hover:underline">Scoping Rules</a>.
      </p>
    </DocPage>
  );
}
