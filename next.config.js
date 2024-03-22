/**
 * @type {import('next').NextConfig}
 */
const nextConfig = {
  output: 'export',

  images: {
    unoptimized: true,
  },

  // Set the base path and asset prefix to match the GitHub Pages subpath
  basePath: '/nextjs-website-test',
  assetPrefix: '/nextjs-website-test/',

  // Optional configurations:
  // Enable trailing slashes in URLs. 
  // This is a personal or project-specific preference.
  trailingSlash: true,
 
  // If you want to disable automatic redirection from `/me` to `/me/`, 
  // you can enable this option. This is also a preference.
  skipTrailingSlashRedirect: true,
 
  // If you wish to change the output directory from `out` to something else,
  // you can specify it here. This might be useful for specific deployment configurations.
  // distDir: 'dist',
}

module.exports = nextConfig
