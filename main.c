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
    TAG("header", "")
    {
        TEXT("span", sitename, "");

        struct
        {
            char *txt;
            char *attribs;
        } links[] = {
            {"home", "href=\"#home\""},
            {"about", "href=\"#about\""},
            {"github", "href=\"https://github.com/tezza48\""},
        };

        TAG("nav", "class=\"navbar\"")
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
    TAG("div", "class=\"hero\"")
    {
        TEXT("h1", "Tezza 48", "");
        TEXT("p", "Server Programmer by day | Everything Programmer by night", "");
    }
    TAG("div", "class=\"section\"")
    {
    }
}

void render_aboutpage(sb_t *sb)
{
    TEXT("h2", "This is the About page", "");
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
            ESC("link", "rel=\"stylesheet\" type=\"text/css\" href=\"/style.css\"");
        }
        TAG("body", "")
        {
            render_header(sb);
            TAG("div", "id=\"home\" class=\"page\"")
            {
                render_homepage(sb);
            }
            TAG("div", "id=\"about\" class=\"page\"")
            {
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