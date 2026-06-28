import DocPage from "@/components/DocPage";

export default function LenDoc() {
  return (
    <DocPage title="len()">
      <p>
        The <code className="text-[#A78BFA]">len()</code> built-in function returns the length or size of a value. 
        It works with arrays, strings, and objects.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Syntax</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`len(value)`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Array length</h2>
      <p>Returns the number of elements in an array:</p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let arr = [10, 20, 30];
print(len(arr));      // 3

let empty = [];
print(len(empty));    // 0`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">String length</h2>
      <p>Returns the number of characters in a string:</p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let s = "hello";
print(len(s));        // 5

let empty = "";
print(len(empty));    // 0`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Object field count</h2>
      <p>Returns the number of fields in an object:</p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`class Point {
    let x = 10;
    let y = 20;
    let z = 30;
}

let p = new Point();
print(len(p));        // 3`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Common use case</h2>
      <p>Iterating with index using <code className="text-[#A78BFA]">len()</code>:</p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let arr = ["a", "b", "c"];

@unsafe for (let i = 0; i < len(arr); i = i + 1) {
    print("Index", i, ":", arr[i]);
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Error handling</h2>
      <p className="text-sm text-yellow-300/80">
        ⚠️ Calling <code className="text-[#A78BFA]">len()</code> on unsupported types (like integers or booleans) 
        throws a runtime error.
      </p>
    </DocPage>
  );
}
