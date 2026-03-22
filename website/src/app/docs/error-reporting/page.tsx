import DocPage from "@/components/DocPage";

export default function ErrorReportingDoc() {
  return (
    <DocPage title="Error Reporting">
      <p>
        When VDX encounters an error in your code, it shows a clear message with
        the file name, line number, and surrounding source context.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">What you see</h2>
      <p>
        Instead of a raw crash, VDX displays the error like this:
      </p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm text-red-300"><code>{`  error: [VDX] Undefined variable 'z' at line 4
   --> myfile.vdx:4
    |
  3 |  let x = 10;
  4 |  print(z);  <--
  5 |  print(x);
    |`}</code></pre>
      </div>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">What gets shown</h2>
      <ul className="list-disc list-inside space-y-2 text-sm">
        <li>The error message with a <code className="text-[#A78BFA]">[VDX]</code> prefix</li>
        <li>The file name and line number</li>
        <li>The line before, the error line (marked with <code className="text-[#A78BFA]">{"<--"}</code>), and the line after</li>
      </ul>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Common errors</h2>
      <table className="w-full text-sm border border-white/10 rounded-xl overflow-hidden">
        <thead>
          <tr className="bg-[#110827]">
            <th className="text-left p-3 text-white">Error</th>
            <th className="text-left p-3 text-white">Cause</th>
          </tr>
        </thead>
        <tbody>
          <tr className="border-t border-white/10">
            <td className="p-3">Undefined variable</td>
            <td className="p-3">Using a variable not declared with <code className="text-[#A78BFA]">let</code></td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3">Undefined function</td>
            <td className="p-3">Calling a function that doesn{"'"}t exist</td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3">Array index out of bounds</td>
            <td className="p-3">Accessing an index beyond the array size</td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3">Division by zero</td>
            <td className="p-3">Dividing an integer by 0</td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3">Expected expression</td>
            <td className="p-3">Syntax error — unexpected token where an expression was expected</td>
          </tr>
          <tr className="border-t border-white/10">
            <td className="p-3">Loop safety</td>
            <td className="p-3">A <code className="text-[#A78BFA]">while</code> loop iterating too fast without <code className="text-[#A78BFA]">@unsafe</code></td>
          </tr>
        </tbody>
      </table>
    </DocPage>
  );
}
