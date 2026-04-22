#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <memory.h>
#include "sb.h"

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

char *parse_markdown(char *src)
{
#define try_end_list(p_isList)         \
    do                                 \
        if (*(p_isList))               \
        {                              \
            sb_append(&sb, "</ul>\n"); \
            *(p_isList) = false;       \
        }                              \
    while (0)

    sb_t sb = {0};
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

        // Replace the newline with null terminator, so we can lazily substring it.
        if (*src_token == '\n')
        {
            *src_token = '\0';
            // Also we increment past it so the next loop iteration doesn't re parse this line. ONLY if it was not the REAL null terminator
            src_token += 1;
        }

        // If there's actually any content in this line, we render it
        if (strlen(line_start))
        {
            // Determine a tag for the string to be placed in whilst updating the line start
            char *tag = "p";
            // Trim leading space
            // TODO WT: Track the indentation / 2 for tabbing in/out ol and ul tags
            int thisIndent = 0;
            trim_space_count(line_start, &thisIndent);

            tag = classify_tag(&line_start);

            // Check whether we need to start a new list or close an existing one
            if (strcmp(tag, "li") == 0)
            {
                if (!isList)
                {
                    sb_append(&sb, "<ul>\n");
                }
                isList = true;
            }
            else
            {
                try_end_list(&isList);
            }

            trim_space(line_start);
            char *rendered = NULL;

            sb_appendf(&sb, "%*s<%s>%s</%s>\n", ((thisIndent + 1) / 2) * 2, "", tag, line_start, tag);
        }

        int count = 0;
        while (*src_token && *src_token == '\n')
        {
            count++;
            ++src_token;
        }
        if (count > 1)
        {
            // We need to try and end a list before rendering any non list line.
            try_end_list(&isList);
            sb_append(&sb, "<br>\n");
        }

        line_start = src_token;
    }

    // Add in a closing list tag if the last line of the src was a list item.
    if (isList)
    {
        sb_append(&sb, "</ul>\n");
    }

    free(copy);

    return sb_flush(&sb);
}

int main(int argc, char **argv)
{
    // TODO WT: remove this entry point, add a header and integrate into "website"

    size_t buf_cap = 256;
    size_t buf_len = 0;
    char *buf = malloc(buf_cap * sizeof(*buf));
    int c = 0;

    while ((c = fgetc(stdin)) != EOF)
    {
        if (buf_len + 1 > buf_cap)
        {
            buf_cap *= 2;
            buf = realloc(buf, buf_cap * sizeof(*buf));
        }
        buf[buf_len++] = (char)c;
    }

    if (buf)
    {
        buf[buf_len] = 0;
    }

    char *markdown = parse_markdown(buf);
    printf(markdown);

    free(markdown);

    free(buf);
}

#include "sb.c"