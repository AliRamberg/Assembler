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
    if(!isalpha(line[0]))
        return NULL;
    if(strstr(line, ":"))
        return strtok(line, ":");
    return NULL;
}