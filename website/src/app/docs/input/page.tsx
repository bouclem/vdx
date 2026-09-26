import DocPage from "@/components/DocPage";

export default function InputDoc() {
  return (
    <DocPage title="input()">
      <p>
        The <code className="text-[#A78BFA]">input()</code> built-in function reads a line of text from the user 
        via standard input (stdin). Useful for interactive programs and user prompts.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Syntax</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`input()              // Read without prompt
input("message")     // Show message, then read`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Basic usage</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let name = input("Enter your name: ");
print("Hello,", name);`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Interactive example</h2>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`class Calculator {
    print("Simple Calculator");

    // input() returns a string — int()/float() convert it (v0.1.5+)
    let a = float(input("Enter first number: "));
    let b = float(input("Enter second number: "));
    print("Sum:", a + b);
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Without prompt</h2>
      <p>
        Call <code className="text-[#A78BFA]">input()</code> without arguments to read silently:
      </p>
      <div className="bg-[var(--vdx-surface)] rounded-lg p-0 my-4">
        <pre className="text-sm"><code>{`let secret = input();
print("You typed:", secret);`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Return value</h2>
      <p>
        Always returns a <code className="text-[#A78BFA]">string</code>. The trailing newline is stripped.
        Convert to numbers with <code className="text-[#A78BFA]">int()</code> or{" "}
        <code className="text-[#A78BFA]">float()</code> (v0.1.5+).
      </p>
    </DocPage>
  );
}
