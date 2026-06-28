import DocPage from "@/components/DocPage";

export default function ArrayFunctionsDoc() {
  return (
    <DocPage title="push() / pop()">
      <p>
        VDX provides built-in functions for modifying arrays: <code className="text-[#A78BFA]">push()</code> to add 
        elements and <code className="text-[#A78BFA]">pop()</code> to remove them.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">push()</h2>
      <p>Appends a value to the end of an array:</p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`push(array, value)`}</code></pre>
      </div>

      <h2 className="text-xl font-semibold text-white mt-6 mb-3">push() example</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let arr = [1, 2];
push(arr, 3);
print(arr);       // [1, 2, 3]

push(arr, 4);
print(arr);       // [1, 2, 3, 4]`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">pop()</h2>
      <p>Removes and returns the last element from an array:</p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`pop(array)  // Returns the removed element`}</code></pre>
      </div>

      <h2 className="text-xl font-semibold text-white mt-6 mb-3">pop() example</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let arr = [10, 20, 30];
let last = pop(arr);

print(last);      // 30
print(arr);       // [10, 20]`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Stack operations</h2>
      <p>
        Combined, <code className="text-[#A78BFA]">push()</code> and <code className="text-[#A78BFA]">pop()</code> 
        enable stack (LIFO) behavior:
      </p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`class Stack {
    let items = [];
    
    fn pushItem(item) {
        push(items, item);
    }
    
    fn popItem() {
        if (len(items) == 0) {
            return "empty";
        }
        return pop(items);
    }
    
    fn size() {
        return len(items);
    }
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Important notes</h2>
      <ul className="list-disc list-inside space-y-2 text-gray-300">
        <li>Both functions modify the array in-place</li>
        <li>
          <code className="text-[#A78BFA]">pop()</code> on an empty array throws an error
        </li>
        <li>
          The array must be a variable (not a literal): <code className="text-red-400">push([1,2], 3)</code> is invalid
        </li>
      </ul>
    </DocPage>
  );
}
