const GITHUB_REPO = "bouclem/vdx";

type VersionStatus = "current" | "old" | "deprecated";

function getDownloadUrl(version: string) {
  return `https://github.com/${GITHUB_REPO}/releases/download/v${version}/vdx-${version}-win64.msi`;
}

const statusLabel: Record<VersionStatus, string> = {
  current: "current",
  old: "old",
  deprecated: "deprecated",
};

const statusClass: Record<VersionStatus, string> = {
  current: "text-emerald-400",
  old: "text-[--vdx-muted]",
  deprecated: "text-red-400",
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
      "Types: float literals, true/false booleans, optional type annotations",
      "Object instantiation with new, dot access, and methods",
      "C-style for loop and for-in over arrays",
      "Mixed int/float arithmetic with auto-promotion",
    ],
  },
  {
    version: "0.0.7",
    date: "2026-03-22",
    status: "old",
    notes: [
      "Arrays with index access, assignment, len(), push()",
      "Improved error reporting with source context",
      "String indexing",
    ],
  },
  {
    version: "0.0.6",
    date: "2026-03-22",
    status: "old",
    notes: [
      "Loop safety protection",
      "@unsafe annotation",
      "Windows MSI installer with PATH integration",
    ],
  },
];

export default function DownloadPage() {
  const current = versions.find((v) => v.status === "current") ?? null;

  return (
    <div className="max-w-5xl mx-auto px-6 py-16">
      <h1 className="text-3xl font-bold text-white mb-2">Download</h1>
      <p className="text-[--vdx-muted] mb-12">
        Install VDX on Windows. After installation, run{" "}
        <code className="text-[--vdx-accent]">vdx yourfile.vdx</code> from any terminal.
      </p>

      {/* Current release */}
      {current !== null && (
        <section className="mb-16">
          <div className="border border-[--vdx-border] rounded-lg p-6 bg-[--vdx-surface]">
            <div className="flex items-baseline justify-between mb-4">
              <div className="flex items-baseline gap-3">
                <span className="text-xl font-mono font-medium text-white">v{current.version}</span>
                <span className="text-xs font-mono text-emerald-400">current</span>
              </div>
              <span className="text-sm text-[--vdx-muted]">{current.date}</span>
            </div>
            <ul className="text-sm text-[--vdx-muted] mb-6 space-y-1">
              {current.notes.map((note, i) => (
                <li key={i} className="flex gap-2">
                  <span className="text-[--vdx-border]">—</span>
                  {note}
                </li>
              ))}
            </ul>
            <a
              href={getDownloadUrl(current.version)}
              className="inline-flex items-center gap-2 bg-white text-[--vdx-dark] px-4 py-2 rounded-md text-sm font-medium hover:bg-zinc-200 transition-colors"
            >
              <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
                <path d="M21 15v4a2 2 0 01-2 2H5a2 2 0 01-2-2v-4" />
                <polyline points="7 10 12 15 17 10" />
                <line x1="12" y1="15" x2="12" y2="3" />
              </svg>
              Windows Installer (.msi)
            </a>
          </div>
        </section>
      )}

      {/* Version history */}
      <section>
        <h2 className="text-sm font-mono text-[--vdx-muted] uppercase tracking-wider mb-6">All releases</h2>
        <div className="border border-[--vdx-border] rounded-lg divide-y divide-[--vdx-border]">
          {versions.map((v) => {
            const isDownloadable = v.status !== "deprecated";

            return (
              <div
                key={v.version}
                className={`px-5 py-4 flex items-center justify-between ${
                  v.status === "deprecated" ? "opacity-50" : ""
                }`}
              >
                <div className="flex items-baseline gap-3">
                  <span className={`font-mono text-sm text-white ${v.status === "deprecated" ? "line-through" : ""}`}>
                    v{v.version}
                  </span>
                  <span className={`text-xs font-mono ${statusClass[v.status]}`}>
                    {statusLabel[v.status]}
                  </span>
                  <span className="text-xs text-[--vdx-muted]">{v.date}</span>
                </div>
                {isDownloadable ? (
                  <a
                    href={getDownloadUrl(v.version)}
                    className="text-[--vdx-accent] hover:text-white text-sm font-mono"
                  >
                    .msi
                  </a>
                ) : (
                  <a
                    href={`/changelog#v${v.version.replace(/\./g, "")}`}
                    className="text-[--vdx-muted] hover:text-white text-sm font-mono"
                  >
                    changelog
                  </a>
                )}
              </div>
            );
          })}
        </div>
        <p className="text-xs text-[--vdx-muted] mt-4">
          Deprecated versions link to their changelog entry. Downloads are served from{" "}
          <a href={`https://github.com/${GITHUB_REPO}/releases`} className="text-[--vdx-accent] hover:text-white">
            GitHub Releases
          </a>.
        </p>
      </section>
    </div>
  );
}
