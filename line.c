#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h> /* DEBUG */
#include "line.h"
#include "asmbl.h"

char *strsub(char *, size_t, char *);

int 
parse_line(line_t *oline)
{
    char *label = (char *)malloc(sizeof(char)*LABEL_LEN);
    
    /***********************************************\
                        LABELS
    \***********************************************/
    if((label = is_label(oline->line)))
        oline->label = label;
    else
        oline->label = NULL;
    /************************************************/
    /* ZERO_MEMORY(dupline, sizeof(dupline)); */
    /* args = sscanf(line->line, "%s", line->first); */
    /* printf("first = %s", line->first); */
    return 0;
}

int 
is_macro(line_t *oline)
{
    if(oline->label)
    {
        /* check if macro on second word */
    }
    else
    {
        /* check if macro on first word */
    }
    
    return TRUE;
}

/**
 * Check if line contains a label, returns the label name if exists.
 */
char *
is_label(char *line)
{
    char *st, *ch;
    ch  = line;
    while(isspace(*ch++));
    st = ch - 1;
    if(isalpha(*st))
    {
        while(isgraph(*ch) && *ch  != ':')
            ch++;
        /* Reached the end of the label title */
        if(*ch == ':')
        {
            if(ch - st > LABEL_LEN)
            {
                fprintf(stderr, "Label %s is too long.\nMust not exceed LABEL_LEN    %d.\n", strsub(st, ch-st, line), LABEL_LEN);
                return NULL;
            }
            return strsub(st, ch - st, line);
        }
    }
    return NULL;
}

/* Is the line completely blank? */
int
is_whitespace(char *line)
{
    char *ch = line;
    while(*ch != '\0')
    {
        if(!isspace(*ch))
            return 0;
        ch++;
    }
    return 1;
}

/* Is the line a comment? */
int
is_comment(char ch)
{
    return ch == ';';
}

/* Is the line worth parsing at all? */
int 
skipable_line(char *line)
{
    return is_comment(*line) || is_whitespace(line);
}

/**
 * Allocates a new string which contains the characters
 * from pointer of index pos up to len characters, 
 * adds the null terminator at the end of sub.
 * Returns the new substring; on failure returns NULL.
 * NOTE: The function uses dynamically allocated memory,
 * needs to be free()'ed with FREE_SAFE or LINE_FREE()
 */
char *
strsub(char *pos, size_t len, char *str)
{
    int c = 0;
    char *sub = (char *)malloc(sizeof(char) * len);
    if(!sub)
        return NULL;
    while(c < len)
    {
        sub[c] = *pos++;
        c++;
    }
    sub[c] = '\0';

    return sub;
}