#include "sb.h"
#include <stdbool.h>
#include <stdarg.h>

static void sb_append(sb_t *sb, char *str)
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

static void sb_append_alloced(sb_t *sb, char *str)
{
    sb_append(sb, str);
    sb->tail->is_alloc = true;
}

static void sb_appendln(sb_t *sb, char *str)
{
    size_t len = strlen(str);
    char *buf = calloc(len + 2, sizeof(*buf));
    memcpy(buf, str, len);
    buf[len] = '\n';
    buf[len + 1] = '\0';

    sb_append_alloced(sb, buf);
}

static void sb_appendf(sb_t *sb, char *fmt, ...)
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

static char *sb_to_str(sb_t *sb)
{
    char *buf = calloc(sb->str_length + 1, sizeof(*buf));
    sb_node *n = sb->head;
    do
    {
        strcat(buf, n->str);
    } while (n = n->next);
    return buf;
}

static inline void sb_free(sb_t *sb)
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
