#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdint.h>
#include <glob.h>
#include <assert.h>

#include "sb.h"
#include "html.h"
#include "markdown.h"

#include "sb.c"
#include "markdown.c"

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

const char *dist_dir = "dist/";
const char *partial_dir = "partial/";
const char *blog_dir = "blog/";
const char *static_content = "static/*.*";

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
    char *buf = NULL;
    fseek(f, 0, SEEK_END);
    size_t len = ftell(f);
    rewind(f);
    buf = malloc((len + 1) * sizeof(*buf));
    fread(buf, len, sizeof(*buf), f);
    fclose(f);

    return buf;
}

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
        memcpy(f.filename, g.gl_pathv[i], sizeof(f.filename));
        f.src = read_file(f.filename);

        // TODO WT: Would be nice to wrap the blog posts so that styling can be applied to them

        f.rendered = parse_markdown(f.src);
        char *preview_src = strndup(f.src, 200);
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
        snprintf(attribs, 512, "href=\"%.*s.html\"", (int)len, basename);
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
                    snprintf(attribs, 512, "href=\"%.*s.html\"", (int)len, basename);
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
}

void move_static_files(void)
{
    glob_t g;
    glob(static_content, 0, NULL, &g);

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

    // TODO WT: i could free them as i go
    for (size_t i = 0; i < blogs.len; i++)
    {
        blog_file *blog = &blogs.data[i];
        char dist_path[512];
        char *basename = NULL;
        size_t len = 0;
        str_filename_noext(blog->filename, &basename, &len);
        snprintf(dist_path, 512, "dist/%.*s.html", (int)len, basename);

        // TODO WT: Render a Prev, Next section under the blog post

        render_page_to_dist(dist_path, blog->rendered);
    }
    free_blog_files(&blogs);
}

#define iter_argv(argc, argv) (argc) ? (argc--, *argv++) : NULL

int main(int argc, char **argv)
{
    // Ignore the binary name
    iter_argv(argc, argv);

    move_static_files();

    render();

    return 0;
}