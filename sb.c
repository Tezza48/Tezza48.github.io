#include "sb.h"
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

void sb_append(sb_t *sb, char *str)
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

void sb_append_alloced(sb_t *sb, char *str)
{
    sb_append(sb, str);
    sb->tail->is_alloc = true;
}

void sb_appendln(sb_t *sb, char *str)
{
    size_t len = strlen(str);
    char *buf = calloc(len + 2, sizeof(*buf));
    memcpy(buf, str, len);
    buf[len] = '\n';
    buf[len + 1] = '\0';

    sb_append_alloced(sb, buf);
}

void sb_appendf(sb_t *sb, char *fmt, ...)
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

char *_build(sb_t *sb)
{
    char *buf = calloc(sb->str_length + 1, sizeof(*buf));
    sb_node *n = sb->head;
    do
    {
        strcat(buf, n->str);
    } while ((n = n->next, n));
    return buf;
}

char *sb_get(sb_t *sb)
{
    char *buf = _build(sb);

    if (sb->cached_str)
    {
        free(sb->cached_str);
        sb->cached_str = 0;
    }
    sb->cached_str = buf;

    return buf;
}

char *sb_flush(sb_t *sb)
{
    char *buf = _build(sb);
    sb_free(sb);
    return buf;
}

void sb_free(sb_t *sb)
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
    if (sb->cached_str)
    {
        free(sb->cached_str);
        sb->cached_str = 0;
    }
}
