#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char *src = ""
            "# This is a heading 1\n"
            "## This is a heading 2\n"
            "### This is a heading 3\n"
            "#### This is a heading 4\n"
            "##### This is a heading 5\n"
            "###### This is a heading 6\n"
            "This is a paragraph\n"
            " * this is an unordered list\n"
            "\n"
            "\n";

typedef struct ml_node ml_node;

struct ml_node
{
    char *line;
    ml_node *next;
};

typedef struct
{
    ml_node *head;
    ml_node *tail;
} ml_list;

#define list_add(pl) (                                       \
    (pl)->tail = *(                                          \
        (pl)->tail                                           \
            ? &(pl)->tail->next                              \
            : &(pl)->head) = calloc(1, sizeof(*(pl)->head)), \
    (pl)->tail)

#define trim_space(str)     \
    while (isspace(*(str))) \
        str++;

int main(int argc, char **argv)
{
    printf("Full Src: \n\n%s\n", src);

    ml_list lines = {0};
    char *copy = strdup(src);

    for (char *src_token = copy; *src_token;)
    {
        char *line_start = src_token;
        while (*src_token != '\0' && *src_token != '\n')
        {
            src_token++;
        }
        *src_token = '\0';
        src_token++;

        printf("Src Line: len:%zu, %s\n", strlen(line_start), line_start);

        // Determine a tag for the string to be placed in whilst updating the line start
        char *tag = "p";
        trim_space(line_start);

        // Parse as h1 -> h6
        if (*line_start == '#')
        {
            int level = 0;
            while (*++line_start == '#' && level < 6)
            {
                level++;
            }

            char *levels[] = {"h1", "h2", "h3", "h4", "h5", "h6"};

            tag = levels[level];
        }

        trim_space(line_start);

        char *rendered = NULL;
        char *fmt = "<%s>%s</%s>\n";
        size_t len = snprintf(rendered, 0, fmt, tag, line_start, tag);
        rendered = calloc(len + 1, sizeof(*rendered));
        snprintf(rendered, len + 1, fmt, tag, line_start, tag);

        list_add(&lines)->line = rendered;
    }

    for (ml_node *node = lines.head; node;)
    {
        printf("Rendered Line: %s", node->line);
        free(node->line);
        ml_node *next = node->next;
        free(node);
        node = next;
    }

    free(copy);

    return 0;
}