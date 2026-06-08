import DocPage from "@/components/DocPage";

export default function IncrementDecrementPage() {
  return (
    <DocPage title="Increment (++) and Decrement (--)">
      <p className="text-[var(--vdx-muted)] mb-6">
        The increment <code className="text-[var(--vdx-accent)]">++</code> and decrement <code className="text-[var(--vdx-accent)]">--</code> operators add or subtract 1 from a variable. They can be used in prefix form (before the variable) or postfix form (after the variable).
      </p>

      <h2 className="text-white font-medium mt-8 mb-3">Syntax</h2>
      <div className="bg-[var(--vdx-surface)] border border-[var(--vdx-border)] rounded-md p-4 mb-6">
        <code className="text-zinc-300 text-sm">
          {`++variable    // Prefix increment
variable++    // Postfix increment
--variable    // Prefix decrement
variable--    // Postfix decrement`}
        </code>
      </div>

      <h2 className="text-white font-medium mt-8 mb-3">Prefix vs Postfix</h2>
      <p className="text-[var(--vdx-muted)] text-sm mb-4">
        The key difference is when the value is returned:
      </p>
      <ul className="text-[var(--vdx-muted)] text-sm space-y-2 mb-6 list-disc list-inside">
        <li><strong className="text-white">Prefix</strong> (<code className="text-[var(--vdx-accent)]">++x</code>): Increments first, then returns the new value</li>
        <li><strong className="text-white">Postfix</strong> (<code className="text-[var(--vdx-accent)]">x++</code>): Returns the current value, then increments</li>
      </ul>

      <h2 className="text-white font-medium mt-8 mb-3">Examples</h2>
      <p className="text-[var(--vdx-muted)] text-sm mb-3">Basic usage:</p>
      <pre className="bg-[var(--vdx-surface)] border border-[var(--vdx-border)] rounded-md p-4 text-sm text-zinc-300 overflow-x-auto mb-4">
{`let x = 5;
x++;              // x is now 6
print(x);         // Output: 6

let y = 10;
y--;              // y is now 9
print(y);         // Output: 9`}
      </pre>

      <p className="text-[var(--vdx-muted)] text-sm mb-3">Prefix vs Postfix difference:</p>
      <pre className="bg-[var(--vdx-surface)] border border-[var(--vdx-border)] rounded-md p-4 text-sm text-zinc-300 overflow-x-auto mb-4">
{`let a = 5;
let b = ++a;      // a becomes 6, then b = 6
print(a, b);      // Output: 6 6

let c = 5;
let d = c++;      // d = 5, then c becomes 6
print(c, d);      // Output: 6 5`}
      </pre>

      <h2 className="text-white font-medium mt-8 mb-3">In Loops</h2>
      <p className="text-[var(--vdx-muted)] text-sm mb-3">Commonly used in for loops:</p>
      <pre className="bg-[var(--vdx-surface)] border border-[var(--var(--vdx-border))] rounded-md p-4 text-sm text-zinc-300 overflow-x-auto mb-4">
{`// Traditional C-style for loop with increment
for (let i = 0; i < 5; i++) {
    print(i);     // Output: 0, 1, 2, 3, 4
}

// Counting down
for (let i = 5; i > 0; i--) {
    print(i);     // Output: 5, 4, 3, 2, 1
}`}
      </pre>

      <h2 className="text-white font-medium mt-8 mb-3">Game Use Cases</h2>
      <p className="text-[var(--vdx-muted)] text-sm mb-3">Counting lives:</p>
      <pre className="bg-[var(--vdx-surface)] border border-[var(--vdx-border)] rounded-md p-4 text-sm text-zinc-300 overflow-x-auto mb-4">
{`let lives = 3;

fn takeDamage() {
    lives--;
    if (lives <= 0) {
        print("Game Over!");
    } else {
        print("Lives remaining:", lives);
    }
}

takeDamage();     // Lives remaining: 2
takeDamage();     // Lives remaining: 1
takeDamage();     // Game Over!`}
      </pre>

      <p className="text-[var(--vdx-muted)] text-sm mb-3">Collecting points:</p>
      <pre className="bg-[var(--vdx-surface)] border border-[var(--vdx-border)] rounded-md p-4 text-sm text-zinc-300 overflow-x-auto mb-4">
{`let score = 0;

fn collectCoin() {
    score++;
    print("Score:", score);
}

fn collectBonus() {
    // Bonus gives 10 points
    for (let i = 0; i < 10; i++) {
        score++;
    }
    print("Bonus collected! Score:", score);
}`}
      </pre>

      <h2 className="text-white font-medium mt-8 mb-3">Requirements</h2>
      <ul className="text-[var(--vdx-muted)] text-sm space-y-2 list-disc list-inside mb-6">
        <li>The operand must be a variable (not a literal or expression)</li>
        <li>The variable must be numeric (int or float)</li>
        <li>Float variables are incremented by 1.0</li>
        <li>Cannot be used on const variables</li>
      </ul>

      <h2 className="text-white font-medium mt-8 mb-3">Restrictions</h2>
      <pre className="bg-[var(--vdx-surface)] border border-[var(--vdx-border)] rounded-md p-4 text-sm text-zinc-300 overflow-x-auto mb-4">
{`// These will cause errors:
let x = 5++;
        // ^^ Error: expected semicolon

let y = (2 + 3)++;
        // ^^ Error: expected variable name

const z = 10;
z++;    // Error: cannot modify const variable`}
      </pre>

      <h2 className="text-white font-medium mt-8 mb-3">See Also</h2>
      <ul className="text-[var(--vdx-muted)] text-sm space-y-1">
        <li><a href="/docs/for" className="text-[var(--vdx-accent)] hover:underline">for loops</a> — Using ++ in loop control</li>
        <li><a href="/docs/while" className="text-[var(--vdx-accent)] hover:underline">while loops</a> — Manual iteration control</li>
        <li><a href="/docs/operators" className="text-[var(--vdx-accent)] hover:underline">Operators</a> — All available operators</li>
      </ul>
    </DocPage>
  );
}
