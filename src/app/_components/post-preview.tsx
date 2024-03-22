import { type Author } from "@/interfaces/author";
import Link from "next/link";
import Avatar from "./avatar";
import CoverImage from "./cover-image";
import DateFormatter from "./date-formatter";

type Props = {
  title: string;
  coverImage: string;
  date: string;
  excerpt: string;
  author: Author;
  slug: string;
};

export function PostPreview({
  title,
  coverImage,
  date,
  excerpt,
  author,
  slug,
}: Props) {
  // Function to check if slug starts with "https://"
  const isExternalLink = slug.startsWith("https://");

  return (
    <div>
      <div className="mb-5">
        <CoverImage slug={slug} title={title} src={coverImage} />
      </div>
      <h3 className="text-3xl mb-3 leading-snug">
        {isExternalLink ? (
          // Render anchor tag for external link
          <a href={slug} target="_blank" rel="noopener noreferrer" className="hover:underline">
            {title}
          </a>
        ) : (
          // Render internal link for other cases
          <Link href="/posts/[slug]" as={`/posts/${slug}`}>
          //  <a className="hover:underline">{title}</a>
          </Link>
        )}
      </h3>
      <p className="text-lg leading-relaxed mb-4">{excerpt}</p>
    </div>
  );
}