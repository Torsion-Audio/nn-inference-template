import Container from "@/app/_components/container";
import { HeroPost } from "@/app/_components/hero-post";
import { Intro } from "@/app/_components/intro";
import { MoreStories } from "@/app/_components/more-stories";
import { TalkDescription } from "@/app/_components/talk-description";
import { AboutUs } from "@/app/_components/about-us";
import { getAllPosts, getAboutUs } from "@/lib/api";

export default function Index() {
  const allPosts = getAllPosts();

  // Assuming the first post is meant to be the "hero" post
  const heroPost = allPosts[0];

  // Remaining posts for the "More Stories" section
  const morePosts = allPosts.slice(1);

  // Fetching about-us posts
  const aboutUs = getAboutUs();
  
  return (
    <main>
      <Container>
        <Intro />
        {heroPost && (
          <HeroPost
            title={heroPost.title}
            coverImage={heroPost.coverImage}
            date={heroPost.date}
            author={heroPost.author}
            slug={heroPost.slug}
            excerpt={heroPost.excerpt}
          />
        )}
        {morePosts.length > 0 && <TalkDescription posts={morePosts} />}
        {morePosts.length > 0 && <MoreStories posts={morePosts} />}
        {aboutUs.length > 0 && <AboutUs posts={aboutUs} />}
      </Container>
    </main>
  );
}
