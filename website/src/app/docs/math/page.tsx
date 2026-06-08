import DocPage from "@/components/DocPage";

export default function MathDoc() {
  return (
    <DocPage title="Math Module">
      <p>
        The <code>math</code> module provides mathematical functions and constants.
        All functions are accessed via the <code>math.</code> prefix.
      </p>

      <h2>Constants</h2>
      <table>
        <thead>
          <tr>
            <th>Constant</th>
            <th>Value</th>
            <th>Description</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td><code>math.pi</code></td>
            <td>3.14159...</td>
            <td>Ratio of circle circumference to diameter</td>
          </tr>
        </tbody>
      </table>

      <h2>Functions</h2>
      <table>
        <thead>
          <tr>
            <th>Function</th>
            <th>Arguments</th>
            <th>Returns</th>
            <th>Description</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td><code>math.sqrt(x)</code></td>
            <td>number</td>
            <td>float</td>
            <td>Square root of x</td>
          </tr>
          <tr>
            <td><code>math.pow(base, exp)</code></td>
            <td>base, exponent</td>
            <td>float</td>
            <td>Base raised to exponent power</td>
          </tr>
          <tr>
            <td><code>math.abs(x)</code></td>
            <td>number</td>
            <td>number</td>
            <td>Absolute value</td>
          </tr>
          <tr>
            <td><code>math.sin(x)</code></td>
            <td>radians</td>
            <td>float</td>
            <td>Sine of angle</td>
          </tr>
          <tr>
            <td><code>math.cos(x)</code></td>
            <td>radians</td>
            <td>float</td>
            <td>Cosine of angle</td>
          </tr>
          <tr>
            <td><code>math.tan(x)</code></td>
            <td>radians</td>
            <td>float</td>
            <td>Tangent of angle</td>
          </tr>
          <tr>
            <td><code>math.floor(x)</code></td>
            <td>number</td>
            <td>float</td>
            <td>Round down to nearest integer</td>
          </tr>
          <tr>
            <td><code>math.ceil(x)</code></td>
            <td>number</td>
            <td>float</td>
            <td>Round up to nearest integer</td>
          </tr>
          <tr>
            <td><code>math.round(x)</code></td>
            <td>number</td>
            <td>number</td>
            <td>Round to nearest integer</td>
          </tr>
          <tr>
            <td><code>math.min(a, b, ...)</code></td>
            <td>2+ numbers</td>
            <td>number</td>
            <td>Smallest value</td>
          </tr>
          <tr>
            <td><code>math.max(a, b, ...)</code></td>
            <td>2+ numbers</td>
            <td>number</td>
            <td>Largest value</td>
          </tr>
          <tr>
            <td><code>math.random()</code></td>
            <td>none</td>
            <td>float</td>
            <td>Random number 0.0 to 1.0</td>
          </tr>
          <tr>
            <td><code>math.random(max)</code></td>
            <td>int max</td>
            <td>int</td>
            <td>Random int 0 to max-1</td>
          </tr>
          <tr>
            <td><code>math.random(min, max)</code></td>
            <td>int min, int max</td>
            <td>int</td>
            <td>Random int min to max (inclusive)</td>
          </tr>
        </tbody>
      </table>

      <h2>Examples</h2>

      <h3>Basic calculations</h3>
      <pre><code>{`class App {
    print(math.sqrt(16));        // 4.0
    print(math.pow(2, 10));      // 1024.0
    print(math.abs(-42));        // 42
    print(math.pi * 2);          // 6.283185...
}`}</code></pre>

      <h3>Trigonometry</h3>
      <pre><code>{`class App {
    const PI = math.pi;
    
    print(math.sin(PI / 2));     // 1.0 (90 degrees)
    print(math.cos(PI));         // -1.0 (180 degrees)
    print(math.tan(0));          // 0.0
}`}</code></pre>

      <h3>Rounding</h3>
      <pre><code>{`class App {
    print(math.floor(3.7));      // 3.0
    print(math.ceil(3.2));       // 4.0
    print(math.round(3.5));     // 4
}`}</code></pre>

      <h3>Min / Max</h3>
      <pre><code>{`class App {
    print(math.min(10, 5));              // 5
    print(math.max(10, 5));              // 10
    print(math.min(3, 1, 4, 1, 5));      // 1
    print(math.max(3, 1, 4, 1, 5));      // 5
}`}</code></pre>

      <h3>Random numbers</h3>
      <pre><code>{`class App {
    // Random float 0.0 to 1.0
    print(math.random());        // e.g., 0.8473...
    
    // Random int 0 to 99
    print(math.random(100));   // e.g., 42
    
    // Random int 1 to 6 (dice roll)
    print(math.random(1, 6));   // e.g., 4
}`}</code></pre>

      <h2>Complete example: Dice roller</h2>
      <pre><code>{`class DiceRoller {
    const SIDES = 6;
    const ROLLS = 5;
    
    fn rollDice() {
        print("Rolling", ROLLS, "d", SIDES, "dice:");
        
        @unsafe for (let i = 0; i < ROLLS; i = i + 1) {
            let roll = math.random(1, SIDES);
            print("Roll", i + 1, ":", roll);
        }
    }
    
    rollDice();
}`}</code></pre>

      <h2>Error handling</h2>
      <ul>
        <li><code>math.sqrt()</code> of a negative number throws an error</li>
        <li><code>math.random(max)</code> requires max &gt; 0</li>
        <li><code>math.random(min, max)</code> requires max &gt; min</li>
        <li>All math functions require numeric arguments</li>
      </ul>
    </DocPage>
  );
}
