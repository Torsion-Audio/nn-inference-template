import cn from "classnames";
import Link from "next/link";
import Image from "next/image";

type Props = {
  title: string;
  src: string;
  slug?: string;
};

const CoverImage = ({ title, src, slug }: Props) => {
  const isExternalLink = slug?.startsWith('https');

  const image = (
    <Image
      src={src}
      alt={`Cover Image for ${title}`}
      className={cn("shadow-sm w-full", {
        "hover:shadow-lg transition-shadow duration-200": slug,
      })}
      width={1300}
      height={630}
    />
  );

  return (
    <div className="sm:mx-0">
      {slug ? (
        isExternalLink ? (
          // If the slug starts with 'https', render an <a> tag for an external link
          <a href={slug} aria-label={title} target="_blank" rel="noopener noreferrer">
            {image}
          </a>
        ) : (
          // For internal navigation, use Next.js's <Link> component
          <Link as={`/posts/${slug}`} href="/posts/[slug]" aria-label={title}>
            {image}
          </Link>
        )
      ) : (
        // If no slug is provided, just display the image without wrapping it in a link
        image
      )}
    </div>
  );
};

export default CoverImage;
