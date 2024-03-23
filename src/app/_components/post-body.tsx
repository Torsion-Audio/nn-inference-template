import markdownStyles from "./markdown-styles.module.css";

type Props = {
  content: string;
};

export function PostBody({ content }: Props) {
  return (
    <div className="max-w-2xl mx-auto">
      <div
        className="markdown"
        dangerouslySetInnerHTML={{ __html: content }}
      />
      <iframe 
        src="https://docs.google.com/presentation/d/e/2PACX-1vQ8kVxwsa4YRc9jeCdajiDaRJzmkpLL-hPzVpSNipetQP90AIdL2MzPdCkpcTrnI4sSkV076l93gQkA/embed?start=false&loop=false&delayms=3000" 
        frameBorder="0" 
        width="1440" 
        height="839" 
        allowFullScreen={true} 
        mozallowfullscreen="true" 
        webkitallowfullscreen="true">
      </iframe>
    </div>
  );
}
