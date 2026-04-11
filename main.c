#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef _DEBUG
size_t __allocs = 0;
#define calloc(x, y) (__allocs++, calloc((x), (y)))
#define free(x) (__allocs--, free((x)))

#define _log_allocs() printf("Allocs still alive: %zu\n", __allocs);
#endif
#define _log_allocs()

typedef struct sb_node sb_node;
struct sb_node
{
    char *str;
    bool is_alloc;
    sb_node *next;
};
typedef struct sb
{
    sb_node *head;
    sb_node *tail;
    size_t str_length;
} sb;
static void sb_append(sb *sb, char *str)
{
    sb_node *n = calloc(1, sizeof(*n));
    n->str = str;
    n->is_alloc = false;

    sb->str_length += strlen(str);

    if (sb->head == NULL)
    {
        sb->head = n;
        sb->tail = n;
    }
    else
    {
        sb->tail->next = n;
        sb->tail = n;
    }
}

static void sb_append_alloced(sb *sb, char *str)
{
    sb_append(sb, str);
    sb->tail->is_alloc = true;
}

static void sb_appendln(sb *sb, char *str)
{
    size_t len = strlen(str);
    char *buf = calloc(len + 2, sizeof(*buf));
    memcpy(buf, str, len);
    buf[len] = '\n';
    buf[len + 1] = '\0';

    sb_append_alloced(sb, buf);
}

static void sb_appendf(sb *sb, char *fmt, ...)
{
    va_list args1, args2;
    va_start(args1, fmt);
    va_copy(args2, args1);
    int len = vsnprintf(NULL, 0, fmt, args1);
    char *buf = calloc(len + 1, sizeof(*buf));
    vsnprintf(buf, len + 1, fmt, args2);
    va_end(args1);
    va_end(args2);

    sb_append_alloced(sb, buf);
}

static char *sb_to_str(sb *sb)
{
    char *buf = calloc(sb->str_length + 1, sizeof(*buf));
    sb_node *n = sb->head;
    do
    {
        strcat(buf, n->str);
    } while (n = n->next);
    return buf;
}

static inline void sb_free(sb *sb)
{
    sb_node *curr = sb->head;
    while (curr)
    {
        if (curr->is_alloc)
        {
            free(curr->str);
        }

        sb_node *next = curr->next;
        free(curr);
        curr = next;
    }

    sb->head = NULL;
    sb->tail = NULL;
    sb->str_length = 0;
}

#define TAG(psb, name, attribs)                                       \
    for (                                                             \
        int _once = (sb_appendf((psb), "<%s %s>", name, attribs), 1); \
        _once;                                                        \
        (sb_appendf((psb), "</%s>", name), _once = 0))

#define TEXT(psb, name, innerText, attribs) sb_appendf((psb), "<%s %s>%s</%s>", name, attribs, innerText, name)
#define ESC(psb, name, attribs) sb_appendf((psb), "<%s %s />", name, attribs)

void render(sb *sb)
{
    char *title = "My HTML Website";

    sb_append(sb, "<!DOCTYPE html>\n");
    TAG(sb, "html", "")
    {
        TAG(sb, "head", "")
        {
            TEXT(sb, "title", title, "");
            ESC(sb, "link", "rel=\"stylesheet\" type=\"text/css\" href=\"/style.css\"");
        }
        TAG(sb, "body", "")
        {
            char *pages[] = {
                "home", "about", "github"};
            char *page_attribs[] = {
                "href=\"#home\"", "href=\"#about\"", "href=\"github.com/tezza48\""};
            TAG(sb, "ul", "class=\"navbar\"")
            {
                for (size_t i = 0; i < sizeof(pages) / sizeof(*pages); i++)
                {
                    TAG(sb, "li", "")
                    {
                        TEXT(sb, "a", pages[i], page_attribs[i]);
                    }
                }
            }

            TEXT(sb, "h1", title, "");
            TAG(sb, "div", "id=\"home\" class=\"page\"")
            {
                TEXT(sb, "h2", "This is the Home page", "");
            }
            TAG(sb, "div", "id=\"about\" class=\"page\"")
            {
                TEXT(sb, "h2", "This is the About page", "");
            }
        }
    }
}

int main(int argc, char **argv)
{
    sb sb = {0};

    render(&sb);

    char *buf = sb_to_str(&sb);
    puts(buf);

    sb_free(&sb);
    free(buf);

    _log_allocs();

    return 0;
}