import type { Metadata } from "next";
import "./globals.css";

export const metadata: Metadata = {
  title: "VDX — The Voidware Programming Language",
  description:
    "A fast, safe programming language built for AI and games. By Voidware.",
};

export default function RootLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  return (
    <html lang="en">
      <body className="min-h-screen">
        <nav className="sticky top-0 z-50 border-b border-[var(--vdx-border)] bg-[var(--vdx-dark)]">
          <div className="max-w-5xl mx-auto flex items-center justify-between px-6 h-14">
            <a href="/" className="flex items-center gap-2.5 text-white font-semibold text-base tracking-tight">
              <span className="text-[var(--vdx-accent)] font-mono font-medium text-lg">V</span>
              VDX
            </a>
            <div className="flex items-center gap-8 text-sm">
              <a href="/docs" className="text-[var(--vdx-muted)] hover:text-white">Docs</a>
              <a href="/changelog" className="text-[var(--vdx-muted)] hover:text-white">Changelog</a>
              <a href="/download" className="text-[var(--vdx-muted)] hover:text-white">Download</a>
              <a
                href="/download"
                className="border border-[var(--vdx-border)] hover:border-[var(--vdx-accent)] text-white px-3.5 py-1.5 rounded-md text-sm font-medium transition-colors"
              >
                Get VDX
              </a>
            </div>
          </div>
        </nav>
        <main>{children}</main>
        <footer className="border-t border-[var(--vdx-border)] mt-32">
          <div className="max-w-5xl mx-auto px-6 py-8 flex items-center justify-between text-sm text-[var(--vdx-muted)]">
            <span>© 2026 Voidware</span>
            <div className="flex items-center gap-6">
              <a href="/changelog" className="hover:text-white">Changelog</a>
              <a href="https://github.com/bouclem/vdx" className="hover:text-white">GitHub</a>
              <a href="https://voidware.xyz" className="hover:text-white">voidware.xyz</a>
            </div>
          </div>
        </footer>
      </body>
    </html>
  );
}
