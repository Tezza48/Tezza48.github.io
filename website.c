#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdint.h>
#include <glob.h>
#include <assert.h>
#include <memory.h>

#include <math.h>

#include "sb.h"
#include "html.h"
#include "markdown.h"

#include "sb.c"
#include "markdown.c"

#include <string.h>

#define arr_push(p_arr, rvalue)                                                            \
    do                                                                                     \
    {                                                                                      \
        if (0 == (p_arr)->data)                                                            \
        {                                                                                  \
            (p_arr)->cap = 64;                                                             \
            (p_arr)->data = malloc((p_arr)->cap * sizeof(*(p_arr)->data));                 \
        }                                                                                  \
        if ((p_arr)->len == (p_arr)->cap)                                                  \
        {                                                                                  \
            (p_arr)->cap *= 2;                                                             \
            (p_arr)->data = realloc((p_arr)->data, (p_arr)->cap * sizeof(*(p_arr)->data)); \
        }                                                                                  \
        (p_arr)->data[(p_arr)->len++] = (rvalue);                                          \
    } while (0)

#define arr_pop(p_arr) (assert((p_arr)->len > 0), (p_arr)->data[--(p_arr)->len])

#define arr_free(p_arr) (free((p_arr)->data), (p_arr)->len = 0, (p_arr)->cap = 0)

#define STRINGBUILDER sb

const char *sitename = "Tezza48's page";

static inline size_t size_min(size_t a, size_t b)
{
    return a < b ? a : b;
}
// TODO WT: Swap to slices/views
void str_filename_noext(char *str, char **out, size_t *out_len)
{
    char *basename = strrchr(str, '/') + 1;
    char *dot = strrchr(basename, '.');
    *out = basename;
    *out_len = (size_t)(dot - basename);
}

/// @brief render and alloc a string
/// @param path
/// @return
char *read_file(const char *const path)
{
    FILE *f = fopen(path, "r");
    fseek(f, 0, SEEK_END);
    size_t len = ftell(f);
    rewind(f);
    char *buf = calloc(len + 1, sizeof(*buf));
    fread(buf, len, sizeof(*buf), f);
    fclose(f);

    return buf;
}

#if 0

typedef struct
{
    char filename[256];
    char *src;
    char *rendered;
    char *preview;
} blog_file;
typedef struct
{
    blog_file *data;
    size_t len, cap;
} blog_files;

blog_files load_blog_files()
{
    blog_files files = {0};

    glob_t g;
    glob("blog/*.md", 0, NULL, &g);

    for (int i = (int)g.gl_pathc - 1; i >= 0; i--)
    {
        blog_file f = {0};
        memcpy(f.filename, g.gl_pathv[i], size_min(sizeof(f.filename), strlen(g.gl_pathv[i])));
        f.src = read_file(f.filename);

        // TODO WT: Would be nice to wrap the blog posts so that styling can be applied to them

        f.rendered = parse_markdown(f.src);
        size_t preview_len = size_min(strlen(f.src), 200);
        char *preview_src = strndup(f.src, preview_len);
        f.preview = parse_markdown(preview_src);
        free(preview_src);

        arr_push(&files, f);
    }

    globfree(&g);

    return files;
}

void free_blog_files(blog_files *files)
{
    while (files->len)
    {
        blog_file f = arr_pop(files);
        free(f.src);
        free(f.rendered);
        free(f.preview);
    }
    arr_free(files);
}

void render_header(sb_t *sb)
{
    header("class=\"d-flex justify-content-between align-items-center px-4 py-2 border-bottom\"")
    {
        TEXT("span", sitename, "class=\"fw-semibold\"");

#define DECORATION " class=\"link-light text-decoration-none\""

        struct
        {
            char *txt;
            char *attribs;
        } links[] = {
            {"home", "href=\"/index.html\"" DECORATION},
            {"about", "href=\"/about.html\"" DECORATION},
            {"blog", "href=\"/blog.html\"" DECORATION},
            {"github", "href=\"https://github.com/tezza48\"" DECORATION},
            {"itch.io", "href=\"https://tezza48.itch.io\"" DECORATION},
        };

#undef DECORATION

        TAG("nav", "class=\"d-flex gap-3\"")
        {
            for (size_t i = 0; i < sizeof(links) / sizeof(*links); i++)
            {
                TEXT("a", links[i].txt, links[i].attribs);
            }
        }
    }
}

char *render_index(blog_file latest_blog)
{
    sb_t *sb = &(sb_t){0};
    sb_append_alloced(sb, read_file("partial/index.html"));
    section("")
    {
        h1("Latest Blog Post", "");
        char attribs[512];

        char *basename = NULL;
        size_t len = 0;
        str_filename_noext(latest_blog.filename, &basename, &len);
        snprintf(attribs, 512, "href=\"blog-post.html?blogPost=%.*s\"", (int)len, basename);
        TAG("a", attribs)
        {
            TAG("div", "")
            {
                sb_append(sb, latest_blog.preview);
            }
        }
    }

    return sb_flush(sb);
}

char *render_blog_posts(blog_files *blogs)
{
    sb_t *sb = &(sb_t){0};

    section("")
    {
        h1("Blog Posts", "");
        TAG("ul", "")
        {
            for (size_t i = 0; i < blogs->len; i++)
            {
                blog_file blog = blogs->data[i];
                TAG("li", "class=\"mb-1\" style=\"min-height: 20vh\"")
                {
                    char attribs[512];

                    char *basename = NULL;
                    size_t len = 0;
                    str_filename_noext(blog.filename, &basename, &len);
                    snprintf(attribs, 512, "href=\"blog-post.html?blogPost=%.*s\"", (int)len, basename);
                    TAG("a", attribs)
                    {
                        sb_appendf(sb, blog.preview);
                    }
                }
            }
        }
    }

    return sb_flush(sb);
}

void render_page_to_dist(char *filename, char *content)
{
    sb_t *sb = &(sb_t){0};

    sb_append(sb, "<!DOCTYPE html>");
    html()
    {
        head()
        {
            TEXT("title", "Tezza 48", "");

            ESC("link", "href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-sRIl4kxILFvY47J16cr9ZwB07vP4J8+LH7qKQnuqkuIAvNWLzeN8tE5YBujZqJLB\" crossorigin=\"anonymous\"");
            ESC("link", "rel=\"stylesheet\" type=\"text/css\" href=\"/style.css\"");
        }
        body()
        {
            render_header(sb);
            main_el("class=\"container d-flex flex-column flex-grow-1\"")
            {
                sb_append(sb, content);
            }
        }
    }

    char *rendered = sb_flush(sb);

    FILE *f = fopen(filename, "w+");
    fwrite(rendered, sizeof(char), strlen(rendered), f);
    free(rendered);
}



// char *render_blog_post(blog_file *content, blog_file *prev, blog_file *next)
// {
//     sb_t *sb = &(sb_t){0};

//     TAG("article", "class=\"mt-5\"")
//     {
//         sb_append(sb, content->rendered);
//         TAG("span", "")
//         {
//             size_t num_links = 0;
//             struct link_info
//             {
//                 char *text;
//                 char *filename;
//                 char *classes;
//             } links[2];

//             if (prev)
//             {
//                 links[num_links++] = (struct link_info){"Prev", prev->filename, ""};
//             }
//             if (next)
//             {
//                 links[num_links++] = (struct link_info){"Next", next->filename, "px-5"};
//             }

//             for (size_t i = 0; i < num_links; i++)
//             {
//                 char *base = NULL;
//                 size_t len = 0;
//                 str_filename_noext(links[i].filename, &base, &len);

//                 char attribs[1024];
//                 snprintf(attribs, sizeof(attribs), "href=\"blog-post.html?blogPost=%.*s\" class=\"%s\"", len, base, links[i].classes);

//                 TAG("a", attribs)
//                 {

//                     sb_appendf(sb, "%s: %.*s", links[i].text, len, base);
//                 }
//             }
//         }
//     }

//     return sb_flush(sb);
// }

void render()
{
    blog_files blogs = load_blog_files();

    char *buf = 0;

    buf = render_index(blogs.data[0]);
    render_page_to_dist("dist/index.html", buf);
    free(buf);

    buf = render_blog_posts(&blogs);
    render_page_to_dist("dist/blog.html", buf);
    free(buf);

    buf = read_file("partial/about.html");
    render_page_to_dist("dist/about.html", buf);
    free(buf);

    buf = read_file("partial/blog-post.html");
    render_page_to_dist("dist/blog-post.html", buf);
    free(buf);

    // TODO WT: i could free them as i go
    for (size_t i = 0; i < blogs.len; i++)
    {
        blog_file *blog = &blogs.data[i];
        char dist_path[512];
        char *basename = NULL;
        size_t len = 0;
        str_filename_noext(blog->filename, &basename, &len);
        snprintf(dist_path, 512, "dist/%.*s.html", (int)len, basename);

        blog_file *prev = NULL, *next = NULL;
        if (i > 0)
        {
            next = &blogs.data[i - 1];
        }
        if (blogs.len - 1 > i)
        {
            prev = &blogs.data[i + 1];
        }

        char *blog_page = render_blog_post(blog, prev, next);

        FILE *f = fopen(dist_path, "w+");
        fwrite(blog_page, sizeof(char), strlen(blog_page), f);

        free(blog_page);
    }
    free_blog_files(&blogs);
}

#endif

#define iter_argv(argc, argv) (argc) ? (*(++argv), --argc) : 0

typedef struct {
    char* data;
    size_t len;
} slice;

#define SLICE_FMT "%.*s"
#define SLICE_ARGS(s) (int)(s).len, (s).data

slice slice_from_cstr(char* cstr) {
    return (slice){cstr, strlen(cstr)};
}

typedef struct {
    slice begin;
    slice end;
} template_block;

typedef struct {
    slice before, found, after;
} tag_split_result;

slice tag_find(slice haystack, slice needle) {
    char* found = memmem(haystack.data, haystack.len, needle.data, needle.len);
    if (found) {
        while (*found != '<' && found != haystack.data) found--;
        char* closing = strchr(found, '>') + 1;
        return (slice){found, closing - found};
    }
    return (slice){0};
}

tag_split_result tag_split(slice str, slice tag_name) {
    slice before = str;
    slice found = {0};
    slice after = {0};

    char* location = memmem(str.data, str.len, tag_name.data, tag_name.len);
    if (location) {
        // Find the opening < by moving left
        while (*location != '<' && location != str.data) {
            location--;
        }

        char* closing = strchr(location, '>') + 1;
        size_t tag_len = closing - location;

        found.data = location;
        found.len = tag_len;

        before.len = before.data - found.data;

        after.data = closing + 1;
        after.len = (str.data + str.len) - after.data;
    }
    return (tag_split_result){before, found, after};
}

slice tag_attrib(slice tag, slice attrib_name) {
    char* attrib_loc = memmem(tag.data, tag.len, attrib_name.data, attrib_name.len);
    if (!attrib_loc) return (slice){0};

    // attrib_name="attrib_value"
    attrib_loc += attrib_name.len + 2;
    char* closing_quote = strchr(attrib_loc, '"');
    return (slice){.data = attrib_loc, .len = closing_quote - attrib_loc};
}

// void cstr_prepend(char** pcstr, slice value);

typedef struct {
    char** data;
    size_t len;
    size_t cap;
} partials_stack;

// Recursively load a partial, inserting into the Content tag of any parent elements
char* render_partial(char* filename)
{
    char* file = read_file(filename);
    slice body = slice_from_cstr(file);

    partials_stack stack = {0};

    slice static_template_tagname = slice_from_cstr("StaticTemplate");
    slice static_template_tagname_close = slice_from_cstr("/StaticTemplate");
    slice static_content_tagname = slice_from_cstr("StaticContent /");

    slice parent_open_tag = tag_find(body, static_template_tagname);
    while (parent_open_tag.len != 0) {
        slice parent_name = tag_attrib(body, slice_from_cstr("src"));
        char name[256] = {0};
        memcpy(name, parent_name.data, (parent_name.len < 255) ? parent_name.len : 255);
        char* parent_src = read_file(name);

        arr_push(&stack, parent_src);
        parent_open_tag = tag_find(slice_from_cstr(parent_src), static_template_tagname);
    }

    sb_t sb = (sb_t){0};

    for (size_t i = stack.len; i-- != 0; ) {
        // Push the befores to the string, outwards in
        slice data = slice_from_cstr(stack.data[i]);

        slice static_template_open = tag_find(data, static_template_tagname);
        slice content_tag = tag_find(data, static_content_tagname);

        char* start = static_template_open.data + static_template_open.len;
        if (!start) start = data.data;
        size_t len = content_tag.data - start;

        sb_appendf(&sb, SLICE_FMT, (int)len, start);
    }
    {
        slice static_template_open = tag_find(body, static_template_tagname);
        slice static_template_close = tag_find(body, static_template_tagname_close);
        char* start = static_template_open.data + static_template_open.len;
        size_t len = static_template_close.data - start;
        if (!start) {
            start = body.data;
            len = body.len;
        }

        sb_appendf(&sb, SLICE_FMT, (int)len, start);
    }
    for (size_t i = 0; i < stack.len; i++) {
        // push the afters to the string, inwards out
        slice data = slice_from_cstr(stack.data[i]);

        slice content_tag = tag_find(data, static_content_tagname);
        slice static_template_close = tag_find(body, static_template_tagname_close);

        char* start = content_tag.data + content_tag.len;
        size_t len =
            static_template_close.data
            ? static_template_close.data - start
            : data.len - (start - data.data);

        sb_appendf(&sb, SLICE_FMT, (int)len, start);

        free(stack.data[i]);
    }

    free(file);

    return sb_flush(&sb);
}

void move_static_files(char* static_content_glob, char* dist_dir)
{
    glob_t g;
    glob(static_content_glob, 0, NULL, &g);

    for (size_t i = 0; i < g.gl_pathc; i++)
    {
        char dst_path[512];
        snprintf(dst_path, sizeof(dst_path), "%s%s", dist_dir, strchr(g.gl_pathv[i], '/'));

        char buf[4096];
        size_t bytes_read = 0;
        FILE *fsrc = fopen(g.gl_pathv[i], "rb");
        FILE *fdst = fopen(dst_path, "wb+");
        while ((bytes_read = fread(buf, sizeof(char), sizeof(buf), fsrc), bytes_read))
        {
            fwrite(buf, sizeof(char), bytes_read, fdst);
        }

        fclose(fdst);
        fclose(fsrc);
    }

    globfree(&g);
}

int main(int argc, char **argv)
{
    char* bin_name = *argv;

    char* pages_dir = "partial/";
    char* blog_post_dir = "blog/";
    char* static_content = "static/*.*";
    char* out_dir = "dist/";

    while (iter_argv(argc, argv)) {

        printf("argv: %s\n", *argv);
        if (strcmp(*argv, "--pages") == 0 || strcmp(*argv, "-p") == 0) {
            iter_argv(argc, argv);
            pages_dir = *argv;
        }
        else if (strcmp(*argv, "--blog") == 0) {
            iter_argv(argc, argv);
            blog_post_dir = *argv;
        }
        else if (strcmp(*argv, "--static") == 0) {
            iter_argv(argc, argv);
            static_content = *argv;
        }
        else if (strcmp(*argv, "--out") == 0 || strcmp(*argv, "-o") == 0) {
            iter_argv(argc, argv);
            out_dir = *argv;
        }
        else if (strcmp(*argv, "--help") == 0) {
            printf(
                "%s --pages <string> --blog <string> --static <string> --out <string>\n"
                "Build the static website\n\n"
                "    --pages:  The directory of pages to render.                      default: '%s'\n"
                "    --blog:   The directory of Markdown blog files.                   default: '%s'\n"
                "    --static: Directory of static files to be copied to the output. default: '%s'\n"
                "    --out:    Output directory.                                        default: '%s'\n"
                "    --help:   Prints this help display.\n",
                bin_name,
                pages_dir,
                blog_post_dir,
                static_content,
                out_dir
            );

            exit(0);
        }
    }

    char* current_partial_filename = "partial/index.html";
    char* rendered = render_partial(current_partial_filename);
    char out_name[256] = {0};
    snprintf(out_name, 256, "%s/%s", out_dir, current_partial_filename + strlen(pages_dir));
    FILE* f = fopen(out_name, "wb");
    fwrite(rendered, sizeof(char), strlen(rendered), f);
    fclose(f);
    free(rendered);

    return 0;
}
