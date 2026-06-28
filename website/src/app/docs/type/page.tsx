import DocPage from "@/components/DocPage";

export default function TypeDoc() {
  return (
    <DocPage title="type()">
      <p>
        The <code className="text-[#A78BFA]">type()</code> built-in function returns the type of a value as a string. 
        Useful for debugging, type checking, and conditional logic.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Syntax</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`type(value)`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Return values</h2>
      <table className="w-full text-left border-collapse my-4">
        <thead>
          <tr className="border-b border-white/10">
            <th className="py-2 text-[#A78BFA]">Value type</th>
            <th className="py-2 text-[#A78BFA]">Returns</th>
          </tr>
        </thead>
        <tbody className="text-gray-300">
          <tr className="border-b border-white/5"><td className="py-2">Integer</td><td className="py-2">"int"</td></tr>
          <tr className="border-b border-white/5"><td className="py-2">Float</td><td className="py-2">"float"</td></tr>
          <tr className="border-b border-white/5"><td className="py-2">String</td><td className="py-2">"string"</td></tr>
          <tr className="border-b border-white/5"><td className="py-2">Boolean</td><td className="py-2">"bool"</td></tr>
          <tr className="border-b border-white/5"><td className="py-2">Array</td><td className="py-2">"array"</td></tr>
          <tr className="border-b border-white/5"><td className="py-2">Object</td><td className="py-2">"object"</td></tr>
          <tr><td className="py-2">Void</td><td className="py-2">"void"</td></tr>
        </tbody>
      </table>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Examples</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let x = 42;
let y = 3.14;
let s = "hello";
let b = true;
let arr = [1, 2, 3];

print(type(x));      // "int"
print(type(y));      // "float"
print(type(s));      // "string"
print(type(b));      // "bool"
print(type(arr));    // "array"

let obj = new Point();
print(type(obj));    // "object"`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Use case: Type checking</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`fn process(value) {
    if (type(value) == "array") {
        print("Processing array of length", len(value));
    } else {
        print("Processing single value:", value);
    }
}`}</code></pre>
      </div>
    </DocPage>
  );
}
