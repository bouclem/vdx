import DocPage from "@/components/DocPage";

export default function ReturnDoc() {
  return (
    <DocPage title="Return Values">
      <p>
        <code className="text-[#A78BFA]">return</code> sends a value back from a function
        to the caller.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Syntax</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`return expr;    // return a value
return;         // return void`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Returning a value</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    fn add(a, b) {
        return a + b;
    }

    let result = add(3, 4);
    print(result);    // 7
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Early return</h2>
      <p>
        <code className="text-[#A78BFA]">return</code> exits the function immediately,
        skipping any remaining code:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    fn abs(n) {
        if (n >= 0) {
            return n;
        }
        return 0 - n;
    }

    print(abs(5));     // 5
    print(abs(0 - 3)); // 3
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Return with conditionals</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    fn max(a, b) {
        if (a > b) {
            return a;
        } else {
            return b;
        }
    }

    print("max:", max(10, 20));    // max: 20
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Void return</h2>
      <p>
        Functions without a <code className="text-[#A78BFA]">return</code> statement (or with
        a bare <code className="text-[#A78BFA]">return;</code>) return <code className="text-[#A78BFA]">void</code>:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    fn doWork() {
        print("working...");
        // no return — returns void
    }

    let result = doWork();
    print(result);    // void
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Using return values in expressions</h2>
      <p>
        Return values can be used anywhere an expression is expected:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    fn square(n) { return n * n; }
    fn double(n) { return n * 2; }

    print(square(3) + double(4));    // 9 + 8 = 17
    let x = square(square(2));       // square(4) = 16
}`}</code></pre>
      </div>
    </DocPage>
  );
}
