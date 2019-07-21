#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "line.h"
#include "asmbl.h"

int 
parse_line(line_t *line)
{
    int args = 0;
    char *label = (char *)malloc(sizeof(char)*LABEL_LEN);
    if((label = is_label(line->line)))
    {
        line->label = label;
    }
    else
    {
        safe_free(label)
        line->label = NULL;
    }
    /* args = sscanf(line->line, "%s", line->first); */
    /* printf("first = %s", line->first); */
    return args;
}

int 
is_macro(line_t *line)
{
    if(line->label)
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
    char *ch = line;
    while(isspace(*ch++));
    while(isalpha(*ch++));
    if(*ch == ':')
        return strtok(line, ":");
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