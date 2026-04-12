#pragma once
#include <stdlib.h>

typedef struct sb_node sb_node;
struct sb_node
{
    char *str;
    bool is_alloc;
    sb_node *next;
};
typedef struct sb_s
{
    sb_node *head;
    sb_node *tail;
    size_t str_length;
} sb_t;

static void sb_append(sb_t *sb, char *str);

static void sb_append_alloced(sb_t *sb, char *str);

static void sb_appendln(sb_t *sb, char *str);

static void sb_appendf(sb_t *sb, char *fmt, ...);

static char *sb_to_str(sb_t *sb);

static inline void sb_free(sb_t *sb);
