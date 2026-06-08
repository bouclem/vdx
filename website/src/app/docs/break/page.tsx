import DocPage from "@/components/DocPage";

export default function BreakDoc() {
  return (
    <DocPage title="break">
      <p>
        The <code>break</code> statement exits a loop immediately, skipping any remaining iterations.
      </p>

      <h2>Syntax</h2>
      <pre><code>{`break;`}</code></pre>

      <h2>Usage</h2>
      <p>
        Use <code>break</code> inside{" "}
        <a href="/docs/while">while</a>,{" "}
        <a href="/docs/for">for</a>, or{" "}
        <code>for-in</code> loops to exit early when a condition is met.
      </p>

      <h2>Example: Exit on condition</h2>
      <pre><code>{`class App {
    let found = false;
    let i = 0;
    
    @unsafe while (i < 100) {
        if (i == 42) {
            found = true;
            break;     // exit loop immediately
        }
        i = i + 1;
    }
    
    print("Found at:", i);  // Found at: 42
}`}</code></pre>

      <h2>Example: for-in with break</h2>
      <pre><code>{`class App {
    let items = ["apple", "banana", "cherry", "date"];
    
    for (item in items) {
        if (item == "cherry") {
            print("Found cherry!");
            break;     // stop searching
        }
        print("Checked:", item);
    }
    // Output: Checked: apple
    //         Checked: banana
    //         Found cherry!
}`}</code></pre>

      <h2>Nested loops</h2>
      <p>
        <code>break</code> only exits the innermost loop it is placed in:
      </p>
      <pre><code>{`class App {
    @unsafe for (let i = 0; i < 3; i = i + 1) {
        for (let j = 0; j < 3; j = j + 1) {
            if (j == 1) {
                break;     // exits inner loop only
            }
            print(i, j);
        }
    }
    // Output: 0 0
    //         1 0
    //         2 0
}`}</code></pre>

      <h2>See also</h2>
      <ul>
        <li><a href="/docs/continue">continue</a> — skip to next iteration</li>
        <li><a href="/docs/while">while loops</a></li>
        <li><a href="/docs/for">for loops</a></li>
      </ul>
    </DocPage>
  );
}
