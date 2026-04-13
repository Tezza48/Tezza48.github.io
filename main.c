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

#define html() TAG("html", "")
#define head() TAG("head", "")
#define body() TAG("body", "")

#define header(attribs) TAG("header", attribs)
#define main_el(attribs) TAG("main", attribs)
#define footer(attribs) TAG("footer", attribs)
#define section(attribs) TAG("section", attribs)

#define h1(innerText, attribs) TEXT("h1", innerText, attribs)
#define h2(innerText, attribs) TEXT("h2", innerText, attribs)
#define h3(innerText, attribs) TEXT("h3", innerText, attribs)
#define h4(innerText, attribs) TEXT("h4", innerText, attribs)
#define h5(innerText, attribs) TEXT("h5", innerText, attribs)
#define p(innerText, attribs) TEXT("p", innerText, attribs)

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

void render_homepage(sb_t *sb)
{
    section("class=\"container d-flex flex-column justify-content-center px-4\" style=\"min-height: 50vh\"")
    {
        h1("Tezza 48", "class=\"display-5 fw-semibold mb-1\"");
        p("Server Programmer by day | Everything Programmer by night", "class=\"text-secondary mb-4\"");
    }

    section("class=\"container\" style=\"min-height: 20vh\"")
    {
        p(
            "Here's the plan, I write a bunch of small projects, etc and they never really get "
            "documented so this site will serve as a bit of a devlog/blog or even a scratch "
            "space just to legitimize the time i spend doing random projects.",
            "");

        ESC("hr", "");
    }

    section("class=\"container\"")
    {
        h2("Latest Devlog", "");

        TEXT("marquee", "Coming Soon", "");
    }
}

void render_aboutpage(sb_t *sb)
{
    section("class=\"container p-3\"")
    {
        h2("About", "class=\"mv-3\"");
        p(
            "Currently I work as a backend programmer in iGaming but I've got many years of experience working "
            "on the frontend, creating games in Pixi.js authoring gamplay features, shaders/effects and even "
            "in-house game engines/frameworks.<br />"
            "I also program traditional games, though not as often as i would like. I take part in Global Games Jam "
            "each year and publish those games on <a href=\"http://tezza48.itch.io\">my itch.io page</a>."
            "<br />"
            "Though the majority of my recent projects have been in C i've used a number of languages in my career/personal projects including "
            "C++, Typescript, Python, C# and Rust.",
            "class=\"mv-3\"");

        p(
            "I'll be super honest here: I'm not a <em>real</em> web developer, I avoid HTML and CSS like the plague. "
            "This site is statically generated in C but i'm pretty sure the HTML and CSS are held together by string and bootstrap being pretty OK.",
            "class=\"mv-3\"");
    }
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
        char *buf = sb_to_str(sb);
        fputs(buf, f);
        fclose(f);
        free(buf);
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