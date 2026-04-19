#pragma once
#include <stdlib.h>
#include <stdbool.h>

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
    char *cached_str;
} sb_t;

static void sb_append(sb_t *sb, char *str);

static void sb_append_alloced(sb_t *sb, char *str);

static void sb_appendln(sb_t *sb, char *str);

static void sb_appendf(sb_t *sb, char *fmt, ...);

/// @brief Returns the built string, cached internally
/// @param sb
/// @return
static char *sb_get(sb_t *sb);

static inline void sb_free(sb_t *sb);
