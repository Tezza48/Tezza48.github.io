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

void sb_append(sb_t *sb, char *str);
void sb_append_alloced(sb_t *sb, char *str);
void sb_appendln(sb_t *sb, char *str);
void sb_appendf(sb_t *sb, char *fmt, ...);

void sb_prepend(sb_t *sb, char *str);
void sb_prepend_alloced(sb_t *sb, char *str);
void sb_prependln(sb_t *sb, char *str);
void sb_prependf(sb_t *sb, char *fmt, ...);

/// @brief Returns the built string, cached internally
/// @param sb
/// @return
char *sb_get(sb_t *sb);

/// @brief Returns the built string, gives ownership to the calling code and frees the builder
/// @param sb
/// @return
char *sb_flush(sb_t *sb);

void sb_free(sb_t *sb);
