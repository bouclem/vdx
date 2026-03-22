import DocPage from "@/components/DocPage";

export default function FunctionsDoc() {
  return (
    <DocPage title="Functions (fn)">
      <p>
        Functions are declared with <code className="text-[#A78BFA]">fn</code> and can take
        parameters and return values.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Syntax</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`fn name(param1, param2) {
    // body
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Basic function</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    fn sayHello() {
        print("Hello!");
    }

    sayHello();
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Parameters</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    fn greet(name) {
        print("Hello,", name);
    }

    greet("Alice");    // Hello, Alice
    greet("Bob");      // Hello, Bob
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Return values</h2>
      <p>
        Use <code className="text-[#A78BFA]">return</code> to send a value back from a function.
        See <a href="/docs/return" className="text-[#A78BFA] hover:underline">Return Values</a>.
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    fn add(a, b) {
        return a + b;
    }

    let result = add(3, 4);
    print(result);    // 7
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Functions as expressions</h2>
      <p>Function calls can be used anywhere an expression is expected:</p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    fn double(n) {
        return n * 2;
    }

    print(double(5));           // 10
    print(double(3) + 1);      // 7
    let x = double(double(2)); // 8
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Hoisting</h2>
      <p>
        Functions are registered before any statements run (two-pass execution).
        You can call a function before it appears in the code:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    sayHi();    // works!

    fn sayHi() {
        print("hi");
    }
}`}</code></pre>
      </div>
    </DocPage>
  );
}
