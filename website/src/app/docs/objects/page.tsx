import DocPage from "@/components/DocPage";

export default function ObjectsDoc() {
  return (
    <DocPage title="Objects (new)">
      <p>
        Use <code className="text-[#A78BFA]">new</code> to create object instances from
        classes. Objects have their own fields and methods.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Creating objects</h2>
      <p>
        Define a class as a blueprint, then instantiate it with{" "}
        <code className="text-[#A78BFA]">new ClassName()</code>:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class Point {
    let x: int = 0;
    let y: int = 0;
}

class Main {
    let p = new Point();
    print(p);           // <Point object>
}`}</code></pre>
      </div>
      <p>
        When <code className="text-[#A78BFA]">new</code> is called, VDX runs the class body
        to initialize fields (via <code className="text-[#A78BFA]">let</code>) and registers
        methods (via <code className="text-[#A78BFA]">fn</code>).
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Accessing fields</h2>
      <p>
        Read fields with dot notation:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let p = new Point();
print(p.x);    // 0
print(p.y);    // 0`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Setting fields</h2>
      <p>
        Assign to fields with dot notation:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let p = new Point();
p.x = 10;
p.y = 20;
print(p.x, p.y);   // 10 20`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Methods</h2>
      <p>
        Functions declared in the class become methods on the object. Methods can
        read and modify the object&apos;s fields:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class Point {
    let x: int = 0;
    let y: int = 0;

    fn setXY(nx, ny) {
        x = nx;
        y = ny;
    }

    fn describe() {
        print("Point:", x, y);
    }
}

class Main {
    let p = new Point();
    p.setXY(5, 15);
    p.describe();       // Point: 5 15
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Methods with return</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class Counter {
    let count: int = 0;

    fn increment() {
        count = count + 1;
    }

    fn getCount() {
        return count;
    }
}

class Main {
    let c = new Counter();
    c.increment();
    c.increment();
    print(c.getCount());   // 2
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Multiple instances</h2>
      <p>
        Each <code className="text-[#A78BFA]">new</code> call creates an independent object
        with its own fields:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`class Main {
    let a = new Point();
    let b = new Point();
    a.x = 1;
    b.x = 99;
    print(a.x);    // 1
    print(b.x);    // 99
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Objects are truthy</h2>
      <p>
        Objects are always truthy in boolean context:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`let p = new Point();
if (p) {
    print("object exists");
}`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Top-level vs blueprint</h2>
      <p>
        Classes serve a dual role in VDX:
      </p>
      <ul className="list-disc list-inside space-y-2 text-sm">
        <li>
          As a <span className="text-white">top-level entry point</span> — the class body
          runs immediately when the file is executed
        </li>
        <li>
          As a <span className="text-white">blueprint</span> — used with{" "}
          <code className="text-[#A78BFA]">new</code> to create object instances
        </li>
      </ul>
      <p className="text-sm text-gray-400 mt-2">
        A class can be both. The top-level class (like <code className="text-[#A78BFA]">Main</code>)
        runs its body, while blueprint classes (like <code className="text-[#A78BFA]">Point</code>)
        are used via <code className="text-[#A78BFA]">new</code>.
      </p>
    </DocPage>
  );
}
