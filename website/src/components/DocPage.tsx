export default function DocPage({
  title,
  children,
}: {
  title: string;
  children: React.ReactNode;
}) {
  return (
    <div className="max-w-3xl mx-auto px-6 py-16">
      <nav className="text-sm text-[var(--vdx-muted)] mb-8 font-mono">
        <a href="/docs" className="hover:text-white">docs</a>
        <span className="mx-2">/</span>
        <span className="text-zinc-300">{title.toLowerCase()}</span>
      </nav>
      <h1 className="text-2xl font-bold text-white mb-8">{title}</h1>
      <div className="space-y-5 text-sm text-zinc-300 leading-relaxed [&_code]:text-[var(--vdx-accent)] [&_code]:text-[13px] [&_pre]:bg-[var(--vdx-surface)] [&_pre]:border [&_pre]:border-[var(--vdx-border)] [&_pre]:rounded-lg [&_pre]:p-4 [&_pre]:overflow-x-auto [&_pre]:text-[13px] [&_h2]:text-white [&_h2]:font-semibold [&_h2]:text-base [&_h2]:mt-8 [&_h2]:mb-3 [&_h3]:text-white [&_h3]:font-medium [&_h3]:text-sm [&_h3]:mt-6 [&_h3]:mb-2">
        {children}
      </div>
    </div>
  );
}
