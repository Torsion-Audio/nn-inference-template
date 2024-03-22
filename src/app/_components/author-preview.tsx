import { type Author } from "@/interfaces/author";
import Link from "next/link";
import Avatar from "./avatar";
import CoverImage from "./author-image";
import DateFormatter from "./date-formatter";

type Props = {
  title: string;
  coverImage: string;
  date: string;
  excerpt: string;
  author: Author;
  slug: string;
};

export function AuthorPreview({
  title,
  coverImage,
  date,
  excerpt,
  author,
  slug,
}: Props) { 
  // Function to determine the appropriate links based on the author's name
  const getAuthorLinks = (author: Author) => {
    const { name } = author;
    if (name.startsWith("Valentin")) {
      return {
        github: "https://github.com/vackva",
        linkedin: "https://linkedin.com/in/valentin-ackva-a5ba7124a"
      };
    } else if (name.startsWith("Fares")) {
      return {
        github: "https://github.com/faressc",
        linkedin: "https://www.linkedin.com/in/fares-schulz-850a79270/"
      };
    } else {
      // Default links
      return {
        github: "#",
        linkedin: "#"
      };
    }
  };

  // Get the appropriate links based on the author's name
  const { github, linkedin } = getAuthorLinks(author);

  return (
    <div className="article-summary">
      <div className="mb-5">
        {/* Not passing slug to CoverImage to avoid rendering it as a link */}
        <CoverImage title={title} src={coverImage} />
      </div>
      <h3 className="text-3xl mb-3 leading-snug text-center"> {/* Added text-center class */}
        {title}
      </h3>
      <div className="flex justify-center items-center gap-4 mt-4">
        {/* Using the appropriate links based on the author's name */}
        <a href={github} className="hover:underline text-lg">
          GitHub
        </a>
        <a href={linkedin} className="hover:underline text-lg">
          LinkedIn
        </a>
      </div>
    </div>
  );  
}
