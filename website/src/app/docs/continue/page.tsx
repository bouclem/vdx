import DocPage from "@/components/DocPage";

export default function ContinueDoc() {
  return (
    <DocPage title="continue">
      <p>
        The <code>continue</code> statement skips the rest of the current loop iteration 
        and proceeds to the next one.
      </p>

      <h2>Syntax</h2>
      <pre><code>{`continue;`}</code></pre>

      <h2>Usage</h2>
      <p>
        Use <code>continue</code> inside{" "}
        <a href="/docs/while">while</a>,{" "}
        <a href="/docs/for">for</a>, or{" "}
        <code>for-in</code> loops to skip certain iterations based on a condition.
      </p>

      <h2>Example: Skip even numbers</h2>
      <pre><code>{`class App {
    @unsafe for (let i = 0; i < 10; i = i + 1) {
        if (i % 2 == 0) {
            continue;  // skip even numbers
        }
        print(i);      // only prints odd numbers: 1, 3, 5, 7, 9
    }
}`}</code></pre>

      <h2>Example: for-in with continue</h2>
      <pre><code>{`class App {
    let items = ["apple", "", "banana", "", "cherry"];
    
    for (item in items) {
        if (item == "") {
            continue;  // skip empty strings
        }
        print("Item:", item);
    }
    // Output: Item: apple
    //         Item: banana
    //         Item: cherry
}`}</code></pre>

      <h2>Example: while loop with continue</h2>
      <pre><code>{`class App {
    let i = 0;
    
    @unsafe while (i < 10) {
        i = i + 1;
        
        if (i < 5) {
            continue;  // skip first 4 iterations
        }
        
        print(i);      // prints: 5, 6, 7, 8, 9, 10
    }
}`}</code></pre>

      <h2>Difference from break</h2>
      <table>
        <thead>
          <tr>
            <th>Statement</th>
            <th>Effect</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td><code>break</code></td>
            <td>Exits the loop completely</td>
          </tr>
          <tr>
            <td><code>continue</code></td>
            <td>Skips to next iteration</td>
          </tr>
        </tbody>
      </table>

      <h2>See also</h2>
      <ul>
        <li><a href="/docs/break">break</a> — exit loop completely</li>
        <li><a href="/docs/while">while loops</a></li>
        <li><a href="/docs/for">for loops</a></li>
      </ul>
    </DocPage>
  );
}
