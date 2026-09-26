import DocPage from "@/components/DocPage";

export default function ArraysDoc() {
  return (
    <DocPage title="Arrays">
      <p>
        Arrays let you store multiple values in a single variable. Declare them
        with <code className="text-[#A78BFA]">let</code> and square brackets.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Creating arrays</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let numbers = [1, 2, 3];
let names = ["Alice", "Bob", "Charlie"];
let empty = [];`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Index access</h2>
      <p>
        Access elements by index (starting at 0):
      </p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let arr = [10, 20, 30];
print(arr[0]);    // 10
print(arr[2]);    // 30`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Index assignment</h2>
      <p>
        Change an element by assigning to an index — including nested arrays,
        object fields, and <code className="text-[#A78BFA]">this</code> (v0.1.5+):
      </p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let arr = [1, 2, 3];
arr[1] = 99;
print(arr);       // [1, 99, 3]

let m = [[1, 2], [3, 4]];
m[0][1] = 9;      // v0.1.5+ nested assign
print(m);         // [[1, 9], [3, 4]]

arr[0]++;         // v0.1.5+ — ++ works on any lvalue`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Reference semantics (v0.1.5+)</h2>
      <p>
        Arrays are shared on assignment — copying the variable does{" "}
        <em>not</em> copy the elements:
      </p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let a = [1, 2, 3];
let b = a;
push(b, 4);
print(a);         // [1, 2, 3, 4] — a and b are the same array`}</code></pre>
      </div>
      <p className="text-sm text-gray-400">
        To get an independent copy, rebuild element-by-element (e.g., loop with{" "}
        <code className="text-[#A78BFA]">push</code>).
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">len()</h2>
      <p>
        Get the length of an array with the built-in{" "}
        <code className="text-[#A78BFA]">len()</code> function:
      </p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let arr = [1, 2, 3];
print(len(arr));  // 3`}</code></pre>
      </div>
      <p className="text-sm text-gray-400">
        <code className="text-[#A78BFA]">len()</code> also works on strings:{" "}
        <code className="text-[#A78BFA]">len("hello")</code> returns <code className="text-[#A78BFA]">5</code>.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">push()</h2>
      <p>
        Append a value to an array with{" "}
        <code className="text-[#A78BFA]">push()</code>:
      </p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let arr = [1, 2];
push(arr, 3);
print(arr);       // [1, 2, 3]`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Printing arrays</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let arr = [1, "two", 3];
print(arr);       // [1, "two", 3]`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Truthiness</h2>
      <p>
        Non-empty arrays are truthy, empty arrays are falsy:
      </p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let arr = [1];
if (arr) {
    print("has items");
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Out of bounds</h2>
      <p className="text-sm text-yellow-300/80">
        ⚠️ Accessing an index outside the array range throws a runtime error with the
        index and array size shown.
      </p>
    </DocPage>
  );
}
