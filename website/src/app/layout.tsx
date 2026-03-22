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
      <body className="min-h-screen antialiased">
        <nav className="fixed top-0 w-full z-50 border-b border-white/10 bg-[#0A0118]/80 backdrop-blur-md">
          <div className="max-w-6xl mx-auto flex items-center justify-between px-6 py-4">
            <a href="/" className="flex items-center gap-2 text-white font-bold text-xl">
              <svg width="28" height="28" viewBox="0 0 512 512" fill="none" xmlns="http://www.w3.org/2000/svg">
                <rect width="512" height="512" rx="96" fill="#6C2BD9" />
                <path d="M152 128L256 384L360 128" stroke="white" strokeWidth="48" strokeLinecap="round" strokeLinejoin="round" />
              </svg>
              VDX
            </a>
            <div className="flex items-center gap-6 text-sm text-gray-300">
              <a href="/download" className="hover:text-white transition-colors">Download</a>
              <a href="/docs" className="hover:text-white transition-colors">Docs</a>
              <a
                href="/download"
                className="bg-[#6C2BD9] hover:bg-[#5B21B6] text-white px-4 py-2 rounded-lg transition-colors text-sm font-medium"
              >
                Get VDX
              </a>
            </div>
          </div>
        </nav>
        <main className="pt-20">{children}</main>
        <footer className="border-t border-white/10 mt-24">
          <div className="max-w-6xl mx-auto px-6 py-8 flex items-center justify-between text-sm text-gray-500">
            <span>© 2026 Voidware</span>
            <a href="https://voidware.xyz" className="hover:text-gray-300 transition-colors">
              voidware.xyz
            </a>
          </div>
        </footer>
      </body>
    </html>
  );
}
