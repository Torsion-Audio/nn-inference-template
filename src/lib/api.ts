import { Post } from "@/interfaces/post";
import fs from "fs";
import matter from "gray-matter";
import { join } from "path";

const postsDirectory = join(process.cwd(), "_posts/stories");
const aboutUsDirectory = join(process.cwd(), "_posts/about-us");

export function getPostSlugs(directory = postsDirectory) {
  return fs.readdirSync(directory);
}

export function getPostBySlug(slug: string, directory = postsDirectory) {
  const realSlug = slug.replace(/\.md$/, "");
  const fullPath = join(directory, `${realSlug}.md`);
  const fileContents = fs.readFileSync(fullPath, "utf8");
  const { data, content } = matter(fileContents);

  return { ...data, slug: realSlug, content } as Post;
}

export function getAllPosts(): Post[] {
  const slugs = getPostSlugs();
  const posts = slugs
    .map((slug) => getPostBySlug(slug))
    // sort posts by date in descending order
    .sort((post1, post2) => (post1.date > post2.date ? -1 : 1));
  return posts;
}

// New function to get all posts from the about-us directory
export function getAboutUs(): Post[] {
  const slugs = getPostSlugs(aboutUsDirectory);
  const posts = slugs
    .map((slug) => getPostBySlug(slug, aboutUsDirectory))
    // You might want to adjust the sorting if these posts don't have a date
    // For example, you could sort them alphabetically by slug if there's no date
    .sort((post1, post2) => (post1.date > post2.date ? -1 : 1));
  return posts;
}
