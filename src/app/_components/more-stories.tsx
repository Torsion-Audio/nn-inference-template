import { Post } from "@/interfaces/post";
import { PostPreview } from "./post-preview";

type Props = {
  posts: Post[];
};

export function MoreStories({ posts }: Props) {
  // Function to modify slug based on a specific title condition
  const modifySlug = (slug: string, title: string): string => {
    // Check if the title matches the specific condition
    if (title === "Following Up: New Unified Inference Library") {
      return "https://github.com/tu-studio/anira";
    }

    if (title === "ADC23 Talk: Source Code") {
      return "https://github.com/Torsion-Audio/nn-inference-template";
    }

    if (title === "ADC23 Talk: Watch Video") {
      return "https://www.youtube.com/watch?v=z_RKgHU59r0";
    }

    // If the title doesn't match the specific condition, return the original slug
    return slug;
  };

  return (
    <section>
      <h2 className="mb-8 text-5xl md:text-7xl font-bold tracking-tighter leading-tight">
        Resources
      </h2>
      <div className="grid grid-cols-1 md:grid-cols-2 md:gap-x-16 lg:gap-x-32 gap-y-20 md:gap-y-32 mb-32">
        {posts.map((post) => (
          <PostPreview
            key={post.slug}
            title={post.title}
            coverImage={post.coverImage}
            date={post.date}
            author={post.author}
            slug={modifySlug(post.slug, post.title)} // Pass modified slug
            excerpt={post.excerpt}
          />
        ))}
      </div>
    </section>
  );
}