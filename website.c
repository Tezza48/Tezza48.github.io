#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>

#include "sb.h"
#include "html.h"

#ifdef _DEBUG
size_t __allocs = 0;
#define calloc(x, y) (__allocs++, calloc((x), (y)))
#define free(x) (__allocs--, free((x)))

#define _log_allocs() printf("Allocs still alive: %zu\n", __allocs);
#endif
#define _log_allocs()

#define STRINGBUILDER sb

const char *sitename = "Tezza48's page";

static inline size_t size_min(size_t a, size_t b)
{
    return a < b ? a : b;
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
        free(curr->path);

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

list load_files_in_dir_into_list(const char *const dirname)
{
    list l = {0};
    DIR *dir = opendir(dirname);

    char src_fullpath[512];
    struct dirent *d = NULL;
    while (d = readdir(dir))
    {
        snprintf(src_fullpath, 512, "%s/%s", dirname, d->d_name);

        struct stat s;
        stat(src_fullpath, &s);

        if (!S_ISREG(s.st_mode))
            continue;

        char *fmt = "dist/%s";
        char *p = NULL;
        size_t len = snprintf(p, 0, fmt, d->d_name);
        p = calloc(len + 1, sizeof(*p));
        snprintf(p, len + 1, fmt, d->d_name);

        sb_t *sb = &((sb_t){0});
        char *b = read_file(src_fullpath);
        section("")
        {
            sb_appendf(sb, b);
        }

        list_add(&l, p, sb_get(sb));
        sb_free(sb);
        free(b);
        free(p);
    }

    closedir(dir);

    return l;
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

char *render_index(char *latest_blog_path, char *latest_blog_body)
{
    sb_t *sb = &(sb_t){0};
    sb_append(sb, read_file("partial/index.html"));
    section("")
    {
        h1("Latest Blog Post", "");
        char attribs[512];
        snprintf(attribs, 512, "href=\"%s\"", latest_blog_path + strlen("dist/"));
        section("")
        {
            TAG("a", attribs)
            {
                char preview[204];
                memset(preview, 0, 204);
                memcpy(preview, latest_blog_body, size_min(strlen(latest_blog_body), 200));
                preview[200] = '.';
                preview[201] = '.';
                preview[202] = '.';
                preview[203] = '\0';
                sb_appendf(sb, preview);
            }
        }
    }

    return sb_flush(sb);
}

char *render_blog_posts(list *l)
{
    sb_t *sb = &(sb_t){0};

    section("")
    {
        h1("Blog Posts", "");
        TAG("ul", "")
        {
            for (list_node *node = l->head; node; node = node->next)
            {
                TAG("li", "class=\"mb-1\" style=\"min-height: 20vh\"")
                {
                    char attribs[512];
                    snprintf(attribs, 512, "href=\"%s\"", node->path + strlen("dist/"));
                    TAG("a", attribs)
                    {
                        char preview[204];
                        memset(preview, 0, 204);

                        memcpy(preview, node->body, size_min(strlen(node->body), 200));
                        preview[200] = '.';
                        preview[201] = '.';
                        preview[202] = '.';
                        preview[203] = '\0';
                        sb_appendf(sb, preview);
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

    list blog_posts = load_files_in_dir_into_list("blog/");

    list l = {0};

    for (list_node *node = blog_posts.head; node; node = node->next)
    {
        list_add(&l, node->path, node->body);
    }

    // TODO WT: Arena would pull a lot of weight in this file

    char *index = render_index(blog_posts.head->path, blog_posts.head->body);
    list_add(&l, "dist/index.html", index);
    free(index);

    char *about = read_file("partial/about.html");
    list_add(&l, "dist/about.html", about);
    free(about);

    char *blog = render_blog_posts(&blog_posts);
    list_add(&l, "dist/blog.html", blog);
    free(blog);

    list_free(&blog_posts);

    sb_t *sb = &(sb_t){0};

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