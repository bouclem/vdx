import DocPage from "@/components/DocPage";

export default function FsDoc() {
  return (
    <DocPage title="Filesystem Module">
      <p>
        The <code className="text-[#A78BFA]">fs</code> module provides file I/O operations. 
        Read and write files with simple function calls.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">fs.readFile(path)</h2>
      <p>Reads a file and returns its contents as a string:</p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let content = fs.readFile("data.txt");
print(content);`}</code></pre>
      </div>
      <p className="text-sm text-gray-400 mt-2">
        Throws an error if the file cannot be read.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">fs.writeFile(path, content)</h2>
      <p>Writes a string to a file:</p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`fs.writeFile("output.txt", "Hello, World!");

// Write multiple lines
let data = "Line 1\nLine 2\nLine 3";
fs.writeFile("lines.txt", data);`}</code></pre>
      </div>
      <p className="text-sm text-gray-400 mt-2">
        Creates the file if it doesn't exist, overwrites if it does.
        Throws an error if the file cannot be written.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Example: Save and load config</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class Config {
    fn save(filename, settings) {
        // Convert dict to simple format
        let content = "name=" + settings["name"] + "\n";
        content = content + "value=" + settings["value"];
        fs.writeFile(filename, content);
    }
    
    fn load(filename) {
        return fs.readFile(filename);
    }
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Error handling</h2>
      <p>
        Both functions throw runtime errors on failure (file not found, permission denied, etc.).
        Errors include the filename and line number for debugging.
      </p>
    </DocPage>
  );
}
