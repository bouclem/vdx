import DocPage from "@/components/DocPage";

export default function InstallationDoc() {
  return (
    <DocPage title="Installation">
      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Windows (MSI Installer)</h2>
      <ol className="list-decimal list-inside space-y-3 text-sm">
        <li>
          Go to the <a href="/download" className="text-[#A78BFA] hover:underline">Download page</a>
        </li>
        <li>Download the latest <code className="text-[#A78BFA]">.msi</code> installer</li>
        <li>Run the installer and follow the steps</li>
        <li>
          Open a new terminal and type:
          <div className="bg-[#110827] border border-white/10 rounded-xl p-4 mt-2">
            <code>vdx --version</code>
          </div>
        </li>
      </ol>
      <p className="text-sm text-gray-500 mt-4">
        The installer adds VDX to your system PATH automatically. You may need
        to restart your terminal for the change to take effect.
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Build from Source</h2>
      <p>Requires CMake 3.16+ and a C++17 compiler.</p>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>{`git clone https://github.com/voidware/vdx.git
cd vdx
cmake -B build
cmake --build build`}</code></pre>
      </div>
      <p className="text-sm">
        The binary will be at <code className="text-[#A78BFA]">build/vdx</code> (or{" "}
        <code className="text-[#A78BFA]">build/Debug/vdx.exe</code> on Windows).
      </p>

      <h2 className="text-2xl font-semibold text-white mt-10 mb-4">Run a .vdx file</h2>
      <div className="bg-[#110827] border border-white/10 rounded-xl p-5 my-4">
        <pre className="text-sm"><code>vdx hello.vdx</code></pre>
      </div>
    </DocPage>
  );
}
