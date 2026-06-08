import DocPage from "@/components/DocPage";

export default function ModuloPage() {
  return (
    <DocPage title="Modulo Operator (%)">
      <p className="text-[var(--vdx-muted)] mb-6">
        The modulo operator <code className="text-[var(--vdx-accent)]">%</code> returns the remainder of a division operation. It&apos;s useful for game calculations, circular indexing, and determining even/odd numbers.
      </p>

      <h2 className="text-white font-medium mt-8 mb-3">Syntax</h2>
      <div className="bg-[var(--vdx-surface)] border border-[var(--vdx-border)] rounded-md p-4 mb-6">
        <code className="text-zinc-300 text-sm">
          dividend % divisor
        </code>
      </div>

      <h2 className="text-white font-medium mt-8 mb-3">Examples</h2>
      <p className="text-[var(--vdx-muted)] text-sm mb-3">Basic modulo operations:</p>
      <pre className="bg-[var(--vdx-surface)] border border-[var(--vdx-border)] rounded-md p-4 text-sm text-zinc-300 overflow-x-auto mb-4">
{`let a = 10 % 3;        // a = 1 (10 divided by 3 is 3 remainder 1)
let b = 15 % 5;        // b = 0 (15 divided by 5 is 3 remainder 0)
let c = 7 % 4;         // c = 3 (7 divided by 4 is 1 remainder 3)

print(a, b, c);        // Output: 1 0 3`}
      </pre>

      <h2 className="text-white font-medium mt-8 mb-3">Game Use Cases</h2>
      <p className="text-[var(--vdx-muted)] text-sm mb-3">Checking if a number is even or odd:</p>
      <pre className="bg-[var(--vdx-surface)] border border-[var(--vdx-border)] rounded-md p-4 text-sm text-zinc-300 overflow-x-auto mb-4">
{`let score = 42;
if (score % 2 == 0) {
    print("Even score!");
} else {
    print("Odd score!");
}`}
      </pre>

      <p className="text-[var(--vdx-muted)] text-sm mb-3">Circular array indexing (wrap around):</p>
      <pre className="bg-[var(--vdx-surface)] border border-[var(--vdx-border)] rounded-md p-4 text-sm text-zinc-300 overflow-x-auto mb-4">
{`let colors = ["red", "green", "blue"];
let playerIndex = 5;
let colorIndex = playerIndex % len(colors);
print(colors[colorIndex]);  // Output: blue (index 2)`}
      </pre>

      <p className="text-[var(--vdx-muted)] text-sm mb-3">Limiting a value to a range:</p>
      <pre className="bg-[var(--vdx-surface)] border border-[var(--vdx-border)] rounded-md p-4 text-sm text-zinc-300 overflow-x-auto mb-4">
{`// Keep angle within 0-360 degrees
let angle = 450;
let normalized = angle % 360;
print(normalized);  // Output: 90`}
      </pre>

      <h2 className="text-white font-medium mt-8 mb-3">Float Support</h2>
      <p className="text-[var(--vdx-muted)] text-sm mb-3">
        The modulo operator works with both integers and floats. If either operand is a float, the result is a float:
      </p>
      <pre className="bg-[var(--vdx-surface)] border border-[var(--vdx-border)] rounded-md p-4 text-sm text-zinc-300 overflow-x-auto mb-4">
{`let x = 10.5 % 3;      // x = 1.5 (float result)
let y = 10 % 3.2;      // y = 0.4 (float result)`}
      </pre>

      <h2 className="text-white font-medium mt-8 mb-3">Edge Cases</h2>
      <ul className="text-[var(--vdx-muted)] text-sm space-y-2 list-disc list-inside">
        <li>Division by zero throws a runtime error</li>
        <li>Both operands must be numeric (int or float)</li>
        <li>Negative numbers are handled correctly</li>
      </ul>

      <h2 className="text-white font-medium mt-8 mb-3">See Also</h2>
      <ul className="text-[var(--vdx-muted)] text-sm space-y-1">
        <li><a href="/docs/operators" className="text-[var(--vdx-accent)] hover:underline">Operators</a> — All available operators</li>
        <li><a href="/docs/increment-decrement" className="text-[var(--vdx-accent)] hover:underline">++ and --</a> — Increment and decrement operators</li>
        <li><a href="/docs/math" className="text-[var(--vdx-accent)] hover:underline">Math module</a> — Additional math functions</li>
      </ul>
    </DocPage>
  );
}
