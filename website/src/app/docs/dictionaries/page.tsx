import DocPage from "@/components/DocPage";

export default function DictionariesDoc() {
  return (
    <DocPage title="Dictionaries">
      <p>
        Dictionaries (also called maps or hash maps) store key-value pairs. 
        Keys are strings, and values can be any type.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Creating a dictionary</h2>
      <p>Use curly braces with string keys and values:</p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let user = {"name": "Alice", "age": 30};
let config = {"debug": true, "port": 8080};`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Accessing values</h2>
      <p>Use bracket notation with a string key:</p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let user = {"name": "Alice", "age": 30};
print(user["name"]);     // "Alice"
print(user["age"]);      // 30`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Adding or updating entries</h2>
      <p>Assign to a key to add or update:</p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let user = {"name": "Alice"};
user["age"] = 30;        // Add new key
user["name"] = "Bob";    // Update existing key
print(user);             // {"name": "Bob", "age": 30}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Dictionary length</h2>
      <p>Use <code className="text-[#A78BFA]">len()</code> to get the number of key-value pairs:</p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let user = {"a": 1, "b": 2, "c": 3};
print(len(user));        // 3`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Type annotation</h2>
      <p>Use the <code className="text-[#A78BFA]">dict</code> type:</p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let user: dict = {"name": "Alice"};`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Example: Counting words</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class WordCounter {
    let words = {"hello": 0, "world": 0};
    
    fn count(word) {
        if (word == "hello") {
            words["hello"] = words["hello"] + 1;
        }
        if (word == "world") {
            words["world"] = words["world"] + 1;
        }
    }
    
    fn show() {
        print(words);
    }
}`}</code></pre>
      </div>
    </DocPage>
  );
}
