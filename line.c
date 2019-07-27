#include <string.h>
#include <ctype.h>
#include <assert.h> /* DEBUG */
#include "line.h"
#include "asmbl.h"
#include "misc.h"

char *strsub(char *, size_t , char *);
char *is_label(char *);
char *is_macro(line_t *);

int 
parse_line(line_t *pLINE)
{
    char *label, *macro;
    
    /***********************************************\
                        LABELS
    \***********************************************/
    if((label = is_label(pLINE->line)))
        pLINE->label = label;
    else
        pLINE->label = NULL;
    /************************************************/

    /* strtok label from line, thus taking only the substring */
    if(pLINE->label)
    {
        strtok(pLINE->line, " ");
        pLINE->line = strtok(NULL, "\0");
    }
    /************************************************/

    /***********************************************\
                        MACROS
    \***********************************************/
    if((macro = is_macro(pLINE)))
        pLINE->macro = macro;
    else
        pLINE->macro = NULL;

    if(pLINE->label && pLINE->macro)
        ERROR_MSG("No label is allowed on a macro sentence")
    /************************************************/

    /* args = sscanf(line->line, "%s", line->first); */
    /* printf("first = %s", line->first); */
    return EXIT_SUCCESS;
}

/* Extractes .define macro.  */
char * 
is_macro(line_t *pLINE)
{
    char *ch = pLINE->line;
    char *name = (char *)malloc(sizeof(char)*MACRO_LEN);
    char *data = (char *)malloc(sizeof(char)*LINE_LEN);
    while(isspace(*ch++));
    ch = strtok(ch-1, " ");
    /* .define has been identified */
    if(hash(ch) == HASH_DEFINE)
    {
        name = strtok(NULL, "=");
        printf("name = %s\n", name);
        data = strtok(NULL, "\0");
        printf("data = %s\n", data);
    }
    
    
    /* printf("data = %s\n", data); */
    return NULL;
}

/**
 * Check if line contains a label, returns the label name if exists.
 */
char *
is_label(char *line)
{
    char *st, *ch, *label;
    ch  = line;
    while(isspace(*ch++));
    st = ch - 1;
    if(isalpha(*st))
    {
        /* Iterate over the characters of the line until reaching the end of the label title */
        while(isgraph(*ch) && *ch  != ':')
            ch++;
        /* Reached the end of the label title */
        if(*ch == ':' && *(ch + 1) == ' ')
        {
            label = strsub(st, ch - st, line);

            /* Label is too long */
            if(ch - st > LABEL_LEN)
            {
                ERROR_MSG("Label is too long.")
                SAFE_FREE(label);
                return NULL;
            }
            /* Label is a reserved word */
            if(is_reserved(label))
            {
                ERROR_MSG("Label name is a reserved word")
                SAFE_FREE(label);
                return NULL;
            }
            return label;
        }
    }
    return NULL;
}

/* Is the line completely blank? */
int
is_whitespace(char *line)
{
    while(*line != '\n')
    {
        if(!isspace(*line))
            return FALSE;
        line++;
    }
    return TRUE;
}

/* Is the line a comment? */
int
is_comment(char *line)
{
    return (*line == ';');
}

/* Is the line worth parsing at all? */
int 
skipable_line(char *line)
{
    return (is_comment(line) || is_whitespace(line));
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
