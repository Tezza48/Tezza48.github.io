- [x] Render a partial with parent `StaticTemplate` tags and `StaticContent` to the console
- [x] Render a single partial to the output `dist` dir
- [x] Render a whole directory of partials to the `dist` dir
- [ ] Implement the hooks for rendering Markdown
- [ ] Implement `StaticInsert` - A map of pre rendered content that gets pulled into a partial. (latest blog post, arbitrary rendered HTMLdata, maybe even direct links to other HTML fies)
  * Partial, "LatestBlogPost" is implemented, only the first one in the file is actually rendered
  * Hooks like this ought to be rendered in a pipeline of some sort rather than shimming it directly into the page render loop
  * It also should probably use the `tag_split` method in a loop so i dont need to duplicate the code to move the cursor for building the strings buy hand quite so much
