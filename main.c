#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct
{
    sb_node *head;
    sb_node *tail;
    size_t str_length;
} sb;

static inline void sb_append(sb *sb, char *str)
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

static inline char *sb_to_str(sb *sb)
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

int main(int argc, char **argv)
{
    sb sb = {0};
    sb_append(&sb, "Hello");
    sb_append(&sb, " ");
    sb_append(&sb, "World!");

    char *buf = sb_to_str(&sb);
    puts(buf);

    sb_free(&sb);
    free(buf);

    _log_allocs();

    return 0;
}