import DocPage from "@/components/DocPage";

export default function ConstDoc() {
  return (
    <DocPage title="Constants (const)">
      <p>
        Constants are declared with <code>const</code> and cannot be reassigned after declaration.
        Use them for values that should never change.
      </p>

      <h2>Syntax</h2>
      <pre><code>{`const name = value;
const name: type = value;`}</code></pre>

      <h2>Basic usage</h2>
      <pre><code>{`class App {
    const PI = 3.14159;
    const MAX_USERS = 100;
    const APP_NAME = "VDX";
    
    print(APP_NAME);       // VDX
    print(PI * 2);        // 6.28318
}`}</code></pre>

      <h2>Type annotations</h2>
      <p>
        Like <a href="/docs/variables">variables</a>, constants support optional type annotations:
      </p>
      <pre><code>{`class App {
    const PI: float = 3.14159;
    const MAX_SIZE: int = 1024;
    const GREETING: string = "Hello";
    const ENABLED: bool = true;
}`}</code></pre>

      <h2>Reassignment error</h2>
      <p>
        Attempting to reassign a constant throws a runtime error:
      </p>
      <pre><code>{`class App {
    const MAX = 100;
    MAX = 200;     // Error: Cannot assign to const variable 'MAX'
}`}</code></pre>

      <h2>Constants vs Variables</h2>
      <table>
        <thead>
          <tr>
            <th>Feature</th>
            <th><code>let</code></th>
            <th><code>const</code></th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td>Declaration</td>
            <td><code>let x = 5;</code></td>
            <td><code>const x = 5;</code></td>
          </tr>
          <tr>
            <td>Reassignment</td>
            <td>Allowed</td>
            <td>Not allowed</td>
          </tr>
          <tr>
            <td>Type annotations</td>
            <td>Supported</td>
            <td>Supported</td>
          </tr>
          <tr>
            <td>Use case</td>
            <td>Mutable values</td>
            <td>Fixed values</td>
          </tr>
        </tbody>
      </table>

      <h2>Best practices</h2>
      <ul>
        <li>Use <code>const</code> for configuration values (timeouts, limits, defaults)</li>
        <li>Use <code>const</code> for mathematical constants (PI, E, etc.)</li>
        <li>Use <code>const</code> for fixed strings (error messages, keys)</li>
        <li>Use <code>UPPER_SNAKE_CASE</code> naming for constants</li>
      </ul>

      <h2>Example: Configuration</h2>
      <pre><code>{`class App {
    const API_URL = "https://api.example.com";
    const TIMEOUT_MS = 5000;
    const MAX_RETRIES = 3;
    const VERSION = "1.0.0";
    
    fn connect() {
        print("Connecting to", API_URL);
        print("Timeout:", TIMEOUT_MS, "ms");
    }
    
    connect();
}`}</code></pre>

      <h2>See also</h2>
      <ul>
        <li><a href="/docs/variables">Variables (let)</a></li>
        <li><a href="/docs/types">Types</a></li>
      </ul>
    </DocPage>
  );
}
