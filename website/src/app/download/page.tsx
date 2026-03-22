const versions = [
  {
    version: "0.0.6",
    date: "2026-03-22",
    latest: true,
    notes: [
      "Loop safety protection (blocks fast loops by default)",
      "@unsafe annotation to bypass loop protection",
      "Windows MSI installer with PATH integration",
    ],
  },
];

export default function DownloadPage() {
  return (
    <div className="max-w-4xl mx-auto px-6 py-16">
      <h1 className="text-4xl font-bold text-white mb-4">Download VDX</h1>
      <p className="text-gray-400 mb-12 text-lg">
        Install VDX on your machine. After installation, run{" "}
        <code className="text-[#A78BFA] bg-[#110827] px-2 py-1 rounded">vdx yourfile.vdx</code>{" "}
        from any terminal.
      </p>

      {/* Latest version */}
      <section className="mb-16">
        <h2 className="text-2xl font-semibold text-white mb-6">Latest Release</h2>
        <div className="bg-[#110827] border border-[#6C2BD9]/40 rounded-2xl p-8">
          <div className="flex items-center justify-between mb-4">
            <div>
              <span className="text-3xl font-bold text-white">v{versions[0].version}</span>
              <span className="ml-3 bg-[#6C2BD9] text-white text-xs px-3 py-1 rounded-full font-medium">
                LATEST
              </span>
            </div>
            <span className="text-gray-500 text-sm">{versions[0].date}</span>
          </div>
          <ul className="text-gray-400 text-sm mb-6 space-y-1">
            {versions[0].notes.map((note, i) => (
              <li key={i}>• {note}</li>
            ))}
          </ul>
          <div className="flex gap-4">
            <a
              href={`/releases/vdx-${versions[0].version}-win64.msi`}
              className="bg-[#6C2BD9] hover:bg-[#5B21B6] text-white px-6 py-3 rounded-xl font-medium transition-colors inline-flex items-center gap-2"
            >
              <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
                <path d="M21 15v4a2 2 0 01-2 2H5a2 2 0 01-2-2v-4" />
                <polyline points="7 10 12 15 17 10" />
                <line x1="12" y1="15" x2="12" y2="3" />
              </svg>
              Windows Installer (.msi)
            </a>
          </div>
        </div>
      </section>

      {/* All versions */}
      <section>
        <h2 className="text-2xl font-semibold text-white mb-6">All Versions</h2>
        <div className="space-y-4">
          {versions.map((v) => (
            <div
              key={v.version}
              className="bg-[#110827] border border-white/10 rounded-xl p-6 flex items-center justify-between"
            >
              <div>
                <span className="text-white font-semibold">v{v.version}</span>
                <span className="text-gray-500 text-sm ml-3">{v.date}</span>
                {v.latest && (
                  <span className="ml-2 text-[#A78BFA] text-xs font-medium">latest</span>
                )}
              </div>
              <a
                href={`/releases/vdx-${v.version}-win64.msi`}
                className="text-[#A78BFA] hover:text-white text-sm transition-colors"
              >
                Download .msi
              </a>
            </div>
          ))}
        </div>
        <p className="text-gray-600 text-sm mt-6">
          Older versions will appear here as new releases are published.
        </p>
      </section>
    </div>
  );
}
