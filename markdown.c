#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <memory.h>

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
            "\n"
            "There's a break above this line\n"
            "This line has no trailing newline";

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
#define trim_space_count(str, p_count) \
    *(p_count) = 0;                    \
    while (isspace(*(str)))            \
    {                                  \
        str++;                         \
        *(p_count) += 1;               \
    }

#define trim_char_count(str, c, p_count) \
    *(p_count) = 0;                      \
    while (*(str) == c)                  \
    {                                    \
        str++;                           \
        *(p_count) += 1;                 \
    }

static inline void add_literal(ml_list *list, char *literal)
{
    list_add(list)->line = strdup(literal);
}

char *classify_tag(char **token)
{
    // Parse as h1 -> h6
    if (**token == '#')
    {
        int level = 0;
        while (*++(*token) == '#' && level < 6)
        {
            level++;
        }

        char *levels[] = {"h1", "h2", "h3", "h4", "h5", "h6"};

        return levels[level];
    }
    else if ((*token)[0] == '*' && (*token)[1] == ' ')
    {
        (*token)++;
        return "li";
    }

    return "p";
}

int main(int argc, char **argv)
{
    printf("Full Src: \n\n\"%s\"\n", src);

    ml_list lines = {0};
    // Copy the src string because we're going to be splitting it with null terminators
    // TODO WT: Operate with string slices so that we can avoid allocating a new string to parse.
    char *copy = strdup(src);

    char *line_start = copy;

    bool isList = false;
    int indent = 0;

    for (char *src_token = copy;;)
    {

        // Handle null terminator as a newline to cater for no trailing newline
        if (*src_token != '\n' && *src_token)
        {

            src_token++;
            continue;
        }

        if (!strlen(line_start))
        {
            break;
        }

        // // TODO WT: check for additional repeated newlines, and insert <br>
        // int numNewlines = 0;
        // trim_char_count(src_token, '\n', &numNewlines);
        // if (numNewlines >= 1)
        // {
        //     list_add(&lines)->line = strdup("<br>\n");
        // }

        // Replace the newline with null terminator, so we can lazily substring it.
        if (*src_token == '\n')
        {
            *src_token = '\0';
            // Also we increment past it so the next loop iteration doesnt re parse this line. ONLY if it was not the REAL null terminator
            src_token += 1;
        }
        printf("Src Line: len:%zu, %s\n", strlen(line_start), line_start);

        // Determine a tag for the string to be placed in whilst updating the line start
        char *tag = "p";
        // Trim leading space
        // TODO WT: Track the indentation / 2 for tabbing in/out ol and ul tags
        int thisIndent = 0;
        trim_space_count(line_start, &thisIndent);

        // If there's actually any content in this line, we render it
        if (strlen(line_start))
        {
            tag = classify_tag(&line_start);

            // Check whether we need to start a new list or close an existing one
            if (strcmp(tag, "li") == 0)
            {
                if (!isList)
                {
                    add_literal(&lines, "<ul>\n");
                }
                isList = true;
            }
            else if (isList)
            {
                add_literal(&lines, "</ul>\n");
                isList = false;
            }

            trim_space(line_start);
            char *rendered = NULL;

#define LINE_PRINTF_ARGS "%*s<%s>%s</%s>\n", ((thisIndent + 1) / 2) * 2, "", tag, line_start, tag
            size_t len = snprintf(rendered, 0, LINE_PRINTF_ARGS);
            rendered = calloc(len + 1, sizeof(*rendered));
            snprintf(rendered, len + 1, LINE_PRINTF_ARGS);
#undef LINE_PRINTF_ARGS

            list_add(&lines)->line = rendered;
        }

        line_start = src_token;
    }

    // Add in a closing list tag if the last line of the src was a list item.
    if (isList)
    {
        add_literal(&lines, "</ul>\n");
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
