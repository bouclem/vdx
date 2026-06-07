const GITHUB_REPO = "bouclem/vdx";

type VersionStatus = "current" | "old" | "deprecated";

function getDownloadUrl(version: string) {
  return `https://github.com/${GITHUB_REPO}/releases/download/v${version}/vdx-${version}-win64.msi`;
}

const statusConfig: Record<VersionStatus, { label: string; badgeClass: string }> = {
  current: {
    label: "CURRENT",
    badgeClass: "bg-[#6C2BD9] text-white",
  },
  old: {
    label: "OLD",
    badgeClass: "bg-amber-600/20 text-amber-400 border border-amber-500/30",
  },
  deprecated: {
    label: "DEPRECATED",
    badgeClass: "bg-red-900/20 text-red-400 border border-red-500/30",
  },
};

const versions: {
  version: string;
  date: string;
  status: VersionStatus;
  notes: string[];
}[] = [
  {
    version: "0.0.8",
    date: "2026-03-23",
    status: "current",
    notes: [
      "Types: float literals, true/false booleans, optional type annotations (let x: int = 5;)",
      "new / object instantiation: let obj = new ClassName(); with dot access and methods",
      "for loop: C-style for (let i = 0; i < n; i = i + 1) and for-in over arrays",
      "Mixed int/float arithmetic with auto-promotion",
    ],
  },
  {
    version: "0.0.7",
    date: "2026-03-22",
    status: "old",
    notes: [
      "Arrays: let arr = [1, 2, 3]; with index access, assignment, len(), push()",
      "Improved error reporting with file name, line numbers, and source context",
      "String indexing: str[0]",
    ],
  },
  {
    version: "0.0.6",
    date: "2026-03-22",
    status: "old",
    notes: [
      "Loop safety protection (blocks fast loops by default)",
      "@unsafe annotation to bypass loop protection",
      "Windows MSI installer with PATH integration",
    ],
  },
];

export default function DownloadPage() {
  const current = versions.find((v) => v.status === "current") ?? null;

  return (
    <div className="max-w-4xl mx-auto px-6 py-16">
      <h1 className="text-4xl font-bold text-white mb-4">Download VDX</h1>
      <p className="text-gray-400 mb-12 text-lg">
        Install VDX on your machine. After installation, run{" "}
        <code className="text-[#A78BFA] bg-[#110827] px-2 py-1 rounded">vdx yourfile.vdx</code>{" "}
        from any terminal.
      </p>

      {/* Current version */}
      <section className="mb-16">
        <h2 className="text-2xl font-semibold text-white mb-6">Current Release</h2>
        {current === null ? (
          <div className="bg-[#110827] border border-white/10 rounded-2xl p-8 text-gray-500 text-sm">
            No current release available.
          </div>
        ) : (
          <div className="bg-[#110827] border border-[#6C2BD9]/40 rounded-2xl p-8">
            <div className="flex items-center justify-between mb-4">
              <div>
                <span className="text-3xl font-bold text-white">v{current.version}</span>
                <span className={`ml-3 text-xs px-3 py-1 rounded-full font-medium ${statusConfig.current.badgeClass}`}>
                  {statusConfig.current.label}
                </span>
              </div>
              <span className="text-gray-500 text-sm">{current.date}</span>
            </div>
            <ul className="text-gray-400 text-sm mb-6 space-y-1">
              {current.notes.map((note, i) => (
                <li key={i}>• {note}</li>
              ))}
            </ul>
            <div className="flex gap-4">
              <a
                href={getDownloadUrl(current.version)}
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
        )}
      </section>

      {/* All versions */}
      <section>
        <h2 className="text-2xl font-semibold text-white mb-6">All Versions</h2>
        <div className="space-y-4">
          {versions.map((v) => {
            const config = statusConfig[v.status];
            const isDownloadable = v.status !== "deprecated";

            return (
              <div
                key={v.version}
                className={`bg-[#110827] border rounded-xl p-6 flex items-center justify-between ${
                  v.status === "deprecated" ? "border-white/5 opacity-60" : "border-white/10"
                }`}
              >
                <div className="flex items-center gap-3">
                  <span className={`text-white font-semibold ${v.status === "deprecated" ? "line-through" : ""}`}>
                    v{v.version}
                  </span>
                  <span className={`text-xs px-2 py-0.5 rounded-full font-medium ${config.badgeClass}`}>
                    {config.label}
                  </span>
                  <span className="text-gray-500 text-sm">{v.date}</span>
                </div>
                {isDownloadable ? (
                  <a
                    href={getDownloadUrl(v.version)}
                    className="text-[#A78BFA] hover:text-white text-sm transition-colors"
                  >
                    Download .msi
                  </a>
                ) : (
                  <span className="text-gray-600 text-sm italic">No longer available</span>
                )}
              </div>
            );
          })}
        </div>
        <p className="text-gray-600 text-sm mt-6">
          Deprecated versions are listed for reference but are no longer available for download.
        </p>
      </section>
    </div>
  );
}
