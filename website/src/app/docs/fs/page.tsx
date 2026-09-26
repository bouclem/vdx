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
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let content = fs.readFile("data.txt");
print(content);`}</code></pre>
      </div>
      <p className="text-sm text-gray-400 mt-2">
        Throws an error if the file cannot be read.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">fs.writeFile(path, content)</h2>
      <p>Writes a string to a file:</p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`fs.writeFile("output.txt", "Hello, World!");

// Write multiple lines
let data = "Line 1\nLine 2\nLine 3";
fs.writeFile("lines.txt", data);`}</code></pre>
      </div>
      <p className="text-sm text-gray-400 mt-2">
        Creates the file if it doesn't exist, overwrites if it does.
        Throws an error if the file cannot be written.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">fs.append(path, content) — v0.1.5+</h2>
      <p>Appends a string to the end of a file (creates it if missing):</p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`fs.append("log.txt", "new entry\n");`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">fs.exists(path) — v0.1.5+</h2>
      <p>Returns <code>true</code> if the path exists, <code>false</code> otherwise:</p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`if (fs.exists("config.txt")) {
    print("config found");
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">fs.listDir(path) — v0.1.5+</h2>
      <p>Returns an array of entry names in a directory:</p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let entries = fs.listDir("src");
for (name in entries) {
    print(name);
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">fs.setRoot(dir) — v0.1.5+ (sandbox)</h2>
      <p>
        Sets a sandbox root. Once set, <em>every</em> fs path is canonicalized and must
        resolve inside <code className="text-[#A78BFA]">dir</code> — attempts to escape
        (e.g., via <code className="text-[#A78BFA]">..</code>) throw an error.
        Unset means unrestricted filesystem access (the default, for backwards
        compatibility).
      </p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`fs.setRoot("data");
fs.readFile("data/config.txt");   // OK — inside the sandbox
fs.readFile("../secret.txt");     // error — escapes the root`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Module name shadowing (v0.1.5+)</h2>
      <p>
        A variable named <code className="text-[#A78BFA]">fs</code>,{" "}
        <code className="text-[#A78BFA]">math</code>, or{" "}
        <code className="text-[#A78BFA]">graph</code> shadows the built-in module —
        <code className="text-[#A78BFA]">fs.x</code> then looks up field{" "}
        <code className="text-[#A78BFA]">x</code> on <em>your</em> variable. Use different
        names (e.g., <code className="text-[#A78BFA]">filesys</code>).
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Example: Save and load config</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
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
