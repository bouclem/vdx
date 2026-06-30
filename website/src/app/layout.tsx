import type { Metadata } from "next";
import "./globals.css";

export const metadata: Metadata = {
  title: {
    default: "VDX — The Voidware Programming Language",
    template: "%s — VDX",
  },
  description:
    "A fast, safe programming language built for AI and games. Class-based, interpreted, with built-in safety, type annotations, and data visualization. By Voidware.",
  keywords: [
    "VDX",
    "Voidware",
    "programming language",
    "interpreted language",
    "AI programming",
    "game development",
    "scripting language",
    "data visualization",
    "SVG plotting",
  ],
  authors: [{ name: "Voidware" }],
  creator: "Voidware",
  publisher: "Voidware",
  metadataBase: new URL("https://voidwarelang.xyz"),
  alternates: {
    canonical: "/",
  },
  openGraph: {
    title: "VDX — The Voidware Programming Language",
    description:
      "A fast, safe programming language built for AI and games. Class-based, interpreted, with built-in safety and data visualization.",
    url: "https://voidwarelang.xyz",
    siteName: "VDX",
    type: "website",
    locale: "en_US",
  },
  twitter: {
    card: "summary_large_image",
    title: "VDX — The Voidware Programming Language",
    description:
      "A fast, safe programming language built for AI and games. By Voidware.",
  },
  robots: {
    index: true,
    follow: true,
    googleBot: {
      index: true,
      follow: true,
      "max-image-preview": "large",
      "max-snippet": -1,
    },
  },
  manifest: "/manifest.json",
  icons: {
    icon: "/icon.svg",
    shortcut: "/icon.svg",
    apple: "/icon.svg",
  },
};

export default function RootLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  return (
    <html lang="en">
      <head>
        <script
          type="application/ld+json"
          dangerouslySetInnerHTML={{
            __html: JSON.stringify({
              "@context": "https://schema.org",
              "@type": "SoftwareApplication",
              name: "VDX",
              applicationCategory: "ProgrammingLanguage",
              operatingSystem: "Windows",
              description:
                "A fast, safe programming language built for AI and games. Class-based, interpreted, with built-in safety and data visualization.",
              url: "https://voidwarelang.xyz",
              author: { "@type": "Organization", name: "Voidware" },
              offers: { "@type": "Offer", price: "0", priceCurrency: "USD" },
            }),
          }}
        />
      </head>
      <body className="min-h-screen">
        <nav className="sticky top-0 z-50 border-b border-[var(--vdx-border)] bg-[var(--vdx-dark)]">
          <div className="max-w-5xl mx-auto flex items-center justify-between px-6 h-14">
            <a href="/" className="flex items-center gap-2 text-white font-semibold text-base tracking-tight">
              <svg width="28" height="28" viewBox="0 0 64 64" className="shrink-0">
                <defs>
                  <linearGradient id="navLogo" x1="0" y1="0" x2="64" y2="64" gradientUnits="userSpaceOnUse">
                    <stop offset="0" stopColor="#A78BFA"/>
                    <stop offset="1" stopColor="#7C3AED"/>
                  </linearGradient>
                </defs>
                <rect width="64" height="64" rx="14" fill="#0a0a0a"/>
                <path d="M18 12 C13 12 12 15 12 20 L12 28 C12 31 10 32 8 32 C10 32 12 33 12 36 L12 44 C12 49 13 52 18 52" fill="none" stroke="url(#navLogo)" strokeWidth="3" strokeLinecap="round" strokeLinejoin="round"/>
                <path d="M46 12 C51 12 52 15 52 20 L52 28 C52 31 54 32 56 32 C54 32 52 33 52 36 L52 44 C52 49 51 52 46 52" fill="none" stroke="url(#navLogo)" strokeWidth="3" strokeLinecap="round" strokeLinejoin="round"/>
                <path d="M24 20 L32 44 L40 20" fill="none" stroke="url(#navLogo)" strokeWidth="4.5" strokeLinecap="round" strokeLinejoin="round"/>
              </svg>
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
