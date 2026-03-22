export default function DocPage({
  title,
  children,
}: {
  title: string;
  children: React.ReactNode;
}) {
  return (
    <div className="max-w-3xl mx-auto px-6 py-16">
      <a href="/docs" className="text-[#A78BFA] text-sm mb-6 inline-block hover:underline">
        ← Back to Docs
      </a>
      <h1 className="text-4xl font-bold text-white mb-6">{title}</h1>
      <div className="prose prose-invert max-w-none space-y-6 text-gray-300 leading-relaxed">
        {children}
      </div>
    </div>
  );
}
