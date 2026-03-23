import DocPage from "@/components/DocPage";

export default function ForDoc() {
  return (
    <DocPage title="for Loops">
      <p>
        VDX supports two styles of <code className="text-[#A78BFA]">for</code> loop:
        C-style and for-in (array iteration).
      </p>

      <div className="bg-yellow-900/20 border border-yellow-500/30 rounded-xl p-5 my-4">
        <p className="text-yellow-300 font-semibold mb-2">⚠️ Loop Protection</p>
        <p className="text-yellow-200 text-sm">
          C-style <code className="text-[#A78BFA]">for</code> loops are subject to the same
          loop safety protection as{" "}
          <a href="/docs/while" className="text-[#A78BFA] hover:underline">while</a> loops.
          Use{" "}
          <a href="/docs/unsafe" className="text-[#A78BFA] hover:underline">@unsafe</a>{" "}
          for fast loops.
        </p>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">C-style for loop</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`for (init; condition; update) {
    // body
}`}</code></pre>
      </div>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    @unsafe for (let i = 0; i < 5; i = i + 1) {
        print("i:", i);
    }
    // Output: i: 0, i: 1, i: 2, i: 3, i: 4
}`}</code></pre>
      </div>

      <p>
        The init part can use <code className="text-[#A78BFA]">let</code> to declare a new
        variable, or assign to an existing one. The update part is an assignment
        (no semicolon needed).
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">for-in loop</h2>
      <p>
        Iterate over each element in an array:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`for (variable in array) {
    // body — variable holds the current element
}`}</code></pre>
      </div>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    let fruits = ["apple", "banana", "cherry"];
    for (fruit in fruits) {
        print("fruit:", fruit);
    }
}`}</code></pre>
      </div>
      <p className="text-sm text-gray-400">
        The for-in loop does not have loop safety protection since it always
        terminates (bounded by array length).
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Accumulating with for</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    let sum: float = 0.0;
    let values = [1.5, 2.5, 3.0];
    for (v in values) {
        sum = sum + v;
    }
    print("sum:", sum);   // sum: 7.0
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Nested for loops</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class App {
    @unsafe for (let i = 0; i < 3; i = i + 1) {
        @unsafe for (let j = 0; j < 3; j = j + 1) {
            print(i, j);
        }
    }
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Scoping</h2>
      <p>
        The loop variable in a C-style <code className="text-[#A78BFA]">for</code> is scoped
        to the loop. In a for-in loop, the iteration variable is scoped to each
        iteration. See{" "}
        <a href="/docs/scoping" className="text-[#A78BFA] hover:underline">Scoping Rules</a>.
      </p>
    </DocPage>
  );
}
