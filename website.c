#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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

void render_partial(sb_t *sb, const char *const path)
{
    FILE *f = fopen(path, "r");
    char *buf = NULL;
    fseek(f, 0, SEEK_END);
    size_t len = ftell(f);
    rewind(f);
    buf = calloc(len + 1, sizeof(*buf));
    fread(buf, len, sizeof(*buf), f);
    fclose(f);

    sb_append_alloced(sb, buf);
}

void render_homepage(sb_t *sb)
{
    render_partial(sb, "partial/index.html");
}

void render_aboutpage(sb_t *sb)
{
    render_partial(sb, "partial/about.html");
}

void render()
{
    char *title = "Tezza 48";

    struct
    {
        char *filename;
        void (*page_content)(sb_t *sb);
    } pages[] = {
        {"dist/index.html", render_homepage},
        {"dist/about.html", render_aboutpage}};

    sb_t *sb = &(sb_t){0};

    for (size_t i = 0; i < sizeof(pages) / sizeof(*pages); i++)
    {

        char *filename = pages[i].filename;
        void (*page_content)(sb_t *sb) = pages[i].page_content;

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
                    page_content(sb);
                }
            }
        }

        FILE *f = fopen(filename, "w+");
        fputs(sb_get(sb), f);
        fclose(f);
        sb_free(sb);
    }
}
#undef STRINGBUILDER

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