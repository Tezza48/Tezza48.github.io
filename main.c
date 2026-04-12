#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "sb.h"

#ifdef _DEBUG
size_t __allocs = 0;
#define calloc(x, y) (__allocs++, calloc((x), (y)))
#define free(x) (__allocs--, free((x)))

#define _log_allocs() printf("Allocs still alive: %zu\n", __allocs);
#endif
#define _log_allocs()

#define TAG(name, attribs)                                                    \
    for (                                                                     \
        int _once = (sb_appendf(STRINGBUILDER, "<%s %s>", name, attribs), 1); \
        _once;                                                                \
        (sb_appendf(STRINGBUILDER, "</%s>", name), _once = 0))

#define TEXT(name, innerText, attribs) sb_appendf(STRINGBUILDER, "<%s %s>%s</%s>", name, attribs, innerText, name)
#define ESC(name, attribs) sb_appendf(STRINGBUILDER, "<%s %s />", name, attribs)

#define STRINGBUILDER sb

const char *sitename = "Tezza48's page";

void render_header(sb_t *sb)
{
    TAG("header", "class=\"d-flex justify-content-between align-items-center px-4 py-2 border-bottom\"")
    {
        TEXT("span", sitename, "class=\"fw-semibold\"");

#define DECORATION " class=\"link-light text-decoration-none\""

        struct
        {
            char *txt;
            char *attribs;
        } links[] = {
            {"home", "href=\"#home\"" DECORATION},
            {"about", "href=\"#about\"" DECORATION},
            {"github", "href=\"https://github.com/tezza48\"" DECORATION},
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

void render_homepage(sb_t *sb)
{

    TAG("section", "id=\"home\" class=\"page d-flex flex-column justify-content-center px-4\" style=\"min-height: 50vh\"")
    {
        TEXT("h1", "Tezza 48", "class=\"display-5 fw-semibold mb-1\"");
        TEXT("p", "Server Programmer by day | Everything Programmer by night", "class=\"text-secondary mb-4\"");
    }
}

void render_aboutpage(sb_t *sb)
{

    TAG("section", "id=\"about\" class=\"page\"")
    {
        TEXT("h2", "About", "class=\"mb-3\"");
        TEXT("p", "Some info about what i do", "");
    }
}

void render(sb_t *sb)
{
    char *title = "My HTML Website";

    sb_append(sb, "<!DOCTYPE html>\n");
    TAG("html", "")
    {
        TAG("head", "")
        {
            TEXT("title", title, "");
            ESC("link", "href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-sRIl4kxILFvY47J16cr9ZwB07vP4J8+LH7qKQnuqkuIAvNWLzeN8tE5YBujZqJLB\" crossorigin=\"anonymous\"");
            ESC("link", "rel=\"stylesheet\" type=\"text/css\" href=\"/style.css\"");
        }
        TAG("body", "class=\"d-flex flex-column min-vh-100\"")
        {
            render_header(sb);
            TAG("main", "class=\"container d-flex flex-column flex-grow-1\"")
            {
                render_homepage(sb);
                render_aboutpage(sb);
            }
        }
    }
}
#undef STRINGBUILDER

#define iter_argv(argc, argv, p_str) p_str = (argc) ? (argc--, *argv++) : NULL

int main(int argc, char **argv)
{
    char *bin;

    iter_argv(argc, argv, bin);

    sb_t sb = {0};

    render(&sb);

    char *buf = sb_to_str(&sb);
    puts(buf);

    sb_free(&sb);
    free(buf);

    _log_allocs();

    return 0;
}

#include "sb.c"