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
          <tr>
            <td><code>math.e</code></td>
            <td>2.71828...</td>
            <td>Euler's number (base of natural logarithm)</td>
          </tr>
          <tr>
            <td><code>math.tau</code></td>
            <td>6.28318...</td>
            <td>2π — ratio of circumference to radius</td>
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
            <td>int</td>
            <td>Round down to nearest integer</td>
          </tr>
          <tr>
            <td><code>math.ceil(x)</code></td>
            <td>number</td>
            <td>int</td>
            <td>Round up to nearest integer</td>
          </tr>
          <tr>
            <td><code>math.round(x)</code></td>
            <td>number</td>
            <td>int</td>
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
            <td>Random int 0 to max (inclusive)</td>
          </tr>
          <tr>
            <td><code>math.random(min, max)</code></td>
            <td>int min, int max</td>
            <td>int</td>
            <td>Random int min to max (inclusive)</td>
          </tr>
          <tr>
            <td><code>math.log(x)</code></td>
            <td>number</td>
            <td>float</td>
            <td>Natural logarithm (requires x &gt; 0)</td>
          </tr>
          <tr>
            <td><code>math.log2(x)</code></td>
            <td>number</td>
            <td>float</td>
            <td>Base-2 logarithm</td>
          </tr>
          <tr>
            <td><code>math.log10(x)</code></td>
            <td>number</td>
            <td>float</td>
            <td>Base-10 logarithm</td>
          </tr>
          <tr>
            <td><code>math.exp(x)</code></td>
            <td>number</td>
            <td>float</td>
            <td>Exponential (e^x)</td>
          </tr>
          <tr>
            <td><code>math.cbrt(x)</code></td>
            <td>number</td>
            <td>float</td>
            <td>Cube root (handles negatives)</td>
          </tr>
          <tr>
            <td><code>math.asin(x)</code></td>
            <td>number [-1, 1]</td>
            <td>float</td>
            <td>Inverse sine (radians)</td>
          </tr>
          <tr>
            <td><code>math.acos(x)</code></td>
            <td>number [-1, 1]</td>
            <td>float</td>
            <td>Inverse cosine (radians)</td>
          </tr>
          <tr>
            <td><code>math.atan(x)</code></td>
            <td>number</td>
            <td>float</td>
            <td>Inverse tangent (radians)</td>
          </tr>
          <tr>
            <td><code>math.atan2(y, x)</code></td>
            <td>number, number</td>
            <td>float</td>
            <td>2-argument arctangent (radians)</td>
          </tr>
          <tr>
            <td><code>math.degrees(x)</code></td>
            <td>radians</td>
            <td>float</td>
            <td>Convert radians to degrees</td>
          </tr>
          <tr>
            <td><code>math.radians(x)</code></td>
            <td>degrees</td>
            <td>float</td>
            <td>Convert degrees to radians</td>
          </tr>
          <tr>
            <td><code>math.gcd(a, b)</code></td>
            <td>int, int</td>
            <td>int</td>
            <td>Greatest common divisor</td>
          </tr>
          <tr>
            <td><code>math.sign(x)</code></td>
            <td>number</td>
            <td>int</td>
            <td>Sign function: -1, 0, or 1</td>
          </tr>
          <tr>
            <td><code>math.clamp(v, min, max)</code></td>
            <td>number, number, number</td>
            <td>number</td>
            <td>Clamp value to [min, max] range</td>
          </tr>
          <tr>
            <td><code>math.factorial(n)</code></td>
            <td>int</td>
            <td>int</td>
            <td>Factorial n! (max 12)</td>
          </tr>
          <tr>
            <td><code>math.fibonacci(n)</code></td>
            <td>int</td>
            <td>int</td>
            <td>Fibonacci number F(n) via fast doubling O(log n) (max 46)</td>
          </tr>
          <tr>
            <td><code>math.isPrime(n)</code></td>
            <td>int</td>
            <td>bool</td>
            <td>Primality test using 6k±1 trial division — O(√n)</td>
          </tr>
          <tr>
            <td><code>math.primes(n)</code></td>
            <td>int</td>
            <td>array</td>
            <td>All primes ≤ n via Sieve of Eratosthenes</td>
          </tr>
          <tr>
            <td><code>math.primeCount(n)</code></td>
            <td>int</td>
            <td>int</td>
            <td>Count of primes ≤ n via sieve</td>
          </tr>
          <tr>
            <td><code>math.sort(arr)</code></td>
            <td>array</td>
            <td>array</td>
            <td>Sort numeric array ascending (returns copy)</td>
          </tr>
          <tr>
            <td><code>math.sortDesc(arr)</code></td>
            <td>array</td>
            <td>array</td>
            <td>Sort numeric array descending (returns copy)</td>
          </tr>
          <tr>
            <td><code>math.count(arr, value)</code></td>
            <td>array, any</td>
            <td>int</td>
            <td>Count occurrences of value in array</td>
          </tr>
          <tr>
            <td><code>math.lcm(a, b)</code></td>
            <td>int, int</td>
            <td>int</td>
            <td>Least common multiple</td>
          </tr>
          <tr>
            <td><code>math.sum(arr)</code></td>
            <td>array</td>
            <td>number</td>
            <td>Sum of numeric array elements</td>
          </tr>
          <tr>
            <td><code>math.mean(arr)</code></td>
            <td>array</td>
            <td>float</td>
            <td>Arithmetic mean of array</td>
          </tr>
          <tr>
            <td><code>math.comb(n, k)</code></td>
            <td>int, int</td>
            <td>int</td>
            <td>Binomial coefficient C(n, k)</td>
          </tr>
          <tr>
            <td><code>math.hypot(x, y)</code></td>
            <td>number, number</td>
            <td>float</td>
            <td>Overflow-safe hypotenuse √(x² + y²)</td>
          </tr>
          <tr>
            <td><code>math.lerp(a, b, t)</code></td>
            <td>number, number, number</td>
            <td>float</td>
            <td>Linear interpolation: a + (b - a) * t</td>
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
    print(math.floor(3.7));      // 3
    print(math.ceil(3.2));       // 4
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
    
    // Random int 0 to 100 (inclusive)
    print(math.random(100));   // e.g., 42
    
    // Random int 1 to 6 (dice roll)
    print(math.random(1, 6));   // e.g., 4
}`}</code></pre>

      <h3>Logarithms and exponentials</h3>
      <pre><code>{`print(math.log(2.718281828));  // ~1.0 (natural log)
print(math.log2(8));           // 3.0
print(math.log10(1000));       // 3.0
print(math.exp(1));            // ~2.718... (e^1)
print(math.cbrt(-27));         // -3.0
`}</code></pre>

      <h3>Inverse trigonometry and angle conversion</h3>
      <pre><code>{`print(math.asin(1));           // ~1.5708 (pi/2 radians)
print(math.acos(-1));          // ~3.14159 (pi radians)
print(math.atan(1));           // ~0.7854 (pi/4 radians)
print(math.atan2(1, 1));       // ~0.7854 (45 degrees)
print(math.degrees(math.pi));  // 180.0
print(math.radians(180));      // ~3.14159
`}</code></pre>

      <h3>Number theory and utilities</h3>
      <pre><code>{`print(math.gcd(48, 18));       // 6
print(math.sign(-42));         // -1
print(math.sign(0));           // 0
print(math.clamp(15, 0, 10));  // 10
print(math.clamp(-5, 0, 10));  // 0
print(math.factorial(5));      // 120
print(math.fibonacci(10));     // 55
print(math.fibonacci(46));     // 1836311903 (max int-safe)
`}</code></pre>

      <h3>Primes, sorting, and statistics (v0.1.4)</h3>
      <pre><code>{`print(math.isPrime(17));        // true
print(math.isPrime(18));        // false
print(math.primes(20));         // [2, 3, 5, 7, 11, 13, 17, 19]
print(math.primeCount(100));    // 25
print(math.sort([3, 1, 4, 1, 5]));  // [1, 1, 3, 4, 5]
print(math.sortDesc([3, 1, 4]));    // [4, 3, 1]
print(math.count([1, 2, 1, 3, 1], 1)); // 3
print(math.lcm(4, 6));          // 12
print(math.sum([1, 2, 3, 4])); // 10
print(math.mean([2, 4, 6]));    // 4.0
print(math.comb(5, 2));         // 10
print(math.hypot(3, 4));        // 5.0
print(math.lerp(0, 10, 0.5));   // 5.0
print(math.e);                  // 2.718281...
print(math.tau);                // 6.283185...
`}</code></pre>

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
        <li><code>math.log()</code>, <code>math.log2()</code>, <code>math.log10()</code> require a positive number</li>
        <li><code>math.asin()</code> and <code>math.acos()</code> require argument in range [-1, 1]</li>
        <li><code>math.clamp()</code> requires min &le; max</li>
        <li><code>math.factorial()</code> requires non-negative integer (max 12)</li>
        <li><code>math.fibonacci()</code> requires non-negative integer (max 46)</li>
        <li><code>math.gcd(0, 0)</code> is undefined and throws an error</li>
        <li><code>math.lcm()</code> result must fit in int range</li>
        <li><code>math.comb()</code> requires non-negative integers; result must fit in int range</li>
        <li><code>math.sum()</code> and <code>math.mean()</code> require numeric array elements</li>
        <li><code>math.mean()</code> throws on empty array</li>
        <li><code>math.sort()</code> and <code>math.sortDesc()</code> sort by numeric value</li>
        <li><code>math.random(max)</code> requires max &gt;= 0</li>
        <li><code>math.random(min, max)</code> requires max &gt;= min</li>
        <li>All math functions require numeric arguments</li>
      </ul>
    </DocPage>
  );
}
