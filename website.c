#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdint.h>
#include <glob.h>

#include "sb.h"
#include "html.h"
#include "markdown.h"
#include <assert.h>

#ifdef _DEBUG
size_t __allocs = 0;
#define calloc(x, y) (__allocs++, calloc((x), (y)))
#define malloc(x) (__allocs++, malloc((x)))
#define free(x) (__allocs--, free((x)))

#define _log_allocs() printf("Allocs still alive: %zu\n", __allocs);
#else
#define _log_allocs()
#endif

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

#define arr_free(p_arr) (free((p_arr)->data), (p_arr)->len = 0, (p_arr)->cap = 0, 0)

#define STRINGBUILDER sb

const char *sitename = "Tezza48's page";

static inline size_t size_min(size_t a, size_t b)
{
    return a < b ? a : b;
}
// TODO WT: Swap to slices/views
void str_filename_noext(char *str, char **out_basename, size_t *out_basename_len)
{
    char *basename = strrchr(str, '/') + 1;
    char *dot = strrchr(basename, '.');
    *out_basename = basename;
    *out_basename_len = (size_t)(dot - basename);
}

typedef struct list_node list_node;

struct list_node
{
    char *path;
    char *body;
    list_node *next;
};

typedef struct
{
    list_node *head;
    list_node *tail;
} list;

// Duplicates the path and body
#define list_add(pl, p, b)                                    \
    do                                                        \
    {                                                         \
        list_node **_list_add_slot =                          \
            (pl)->head ? &(pl)->tail->next : &(pl)->head;     \
        *_list_add_slot = calloc(1, sizeof(*_list_add_slot)); \
        (*_list_add_slot)->path = strdup(p);                  \
        (*_list_add_slot)->body = strdup(b);                  \
        (pl)->tail = *_list_add_slot;                         \
    } while (0)

static void list_free(list *l)
{
    list_node *curr = l->head;
    while (curr)
    {
        list_node *next = curr->next;

        free(curr->body);
        curr->body = 0;
        free(curr->path);
        curr->path = 0;

        free(curr);
        curr = next;
    }
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
    buf = calloc(len + 1, sizeof(*buf));
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

    for (size_t i = g.gl_pathc - 1; i != -1; i--)
    {
        blog_file f = {0};
        memcpy(f.filename, g.gl_pathv[i], sizeof(f.filename));
        f.src = read_file(f.filename);

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
    sb_append(sb, read_file("partial/index.html"));
    section("")
    {
        h1("Latest Blog Post", "");
        char attribs[512];

        char *basename = NULL;
        size_t len = 0;
        str_filename_noext(latest_blog.filename, &basename, &len);
        snprintf(attribs, 512, "href=\"%.*s.html\"", len, basename);
        TAG("a", attribs)
        {
            TAG("div", "")
            {
                sb_appendf(sb, latest_blog.preview);
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
                    snprintf(attribs, 512, "href=\"%.*s.html\"", len, basename);
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

void render()
{
    char *title = "Tezza 48";

    blog_files blogs = load_blog_files();

    list l = {0};

    for (size_t i = 0; i < blogs.len; i++)
    {
        blog_file *blog = &blogs.data[i];
        char dist_path[512];
        char *basename = NULL;
        size_t len = 0;
        str_filename_noext(blog->filename, &basename, &len);
        snprintf(dist_path, 512, "dist/%.*s.html", len, basename);
        list_add(&l, dist_path, blog->rendered);
    }

    // TODO WT: Arena would pull a lot of weight in this file

    char *index = render_index(blogs.data[0]);
    list_add(&l, "dist/index.html", index);
    free(index);

    char *about = read_file("partial/about.html");
    list_add(&l, "dist/about.html", about);
    free(about);

    char *blog = render_blog_posts(&blogs);
    list_add(&l, "dist/blog.html", blog);
    free(blog);

    sb_t *sb = &(sb_t){0};

    free_blog_files(&blogs);

    for (list_node *node = l.head; node; node = node->next)
    {
        char *filename = node->path;

        sb_append(sb, "<!DOCTYPE html>");
        html()
        {
            head()
            {
                TEXT("title", title, "");

                ESC("link", "href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-sRIl4kxILFvY47J16cr9ZwB07vP4J8+LH7qKQnuqkuIAvNWLzeN8tE5YBujZqJLB\" crossorigin=\"anonymous\"");
                ESC("link", "rel=\"stylesheet\" type=\"text/css\" href=\"/style.css\"");
            }
            body()
            {
                render_header(sb);
                main_el("class=\"container d-flex flex-column flex-grow-1\"")
                {
                    sb_append(sb, node->body);
                }
            }
        }

        FILE *f = fopen(filename, "w+");
        char *content = sb_get(sb);
        size_t len = strlen(content);
        fputs(content, f);
        fclose(f);
        sb_free(sb);
    }

    list_free(&l);
}

#define iter_argv(argc, argv, p_str) p_str = (argc) ? (argc--, *argv++) : NULL

int main(int argc, char **argv)
{
    char *bin;

    iter_argv(argc, argv, bin);

    render();

    _log_allocs();

    return 0;
}

#include "sb.c"
#include "markdown.c"