#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "pass.h"
#include "asmbl.h"
#include "misc.h"
#include "line.h"



int
first_pass(FILE *fptr, symbol_node **list)
{
    char *line = (char *)malloc(sizeof(char) * (LINE_LEN + 2));
    line_t *pLINE = (line_t *)malloc(sizeof(line_t));
    enum PARSE parse;
    int result = FALSE;

    if(!line || !pLINE)
        return -1;

    line_num = 0;
    IC = 0;  /* Instruction Counter */
    DC = 0;  /* Data Counter */

    /* Reading line by line of the file */
    while (fgets(line, LINE_LEN + 2, fptr) != NULL)
    {
        pLINE->line = line;
        if(line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        line_num++;
        /* Check if line is longer than LINE_LEN */
        if(strlen(line) > LINE_LEN)
        {
            long p = ftell(fptr);
            printf("Line length %ld\n", strlen(line));
            printf("Current location %ld\n", p);
            ERROR_MSG("Line Exceeds max line length.")
            /* TODO EVERYTJOMG */
            fseek(fptr, p + strlen(line), SEEK_SET);
            continue;
        }

        /* Check if line is a whitespace or a comment */
        if(skipable_line(pLINE->line))
            continue;


        /***************** PARSING LINE *****************/
        parse = parse_line(pLINE);                    /**/
        /************************************************/            
        result = encode(parse, pLINE, list);

    if(pLINE->parsed)
        free_symbol(pLINE->parsed);
    }
    LINE_FREE(pLINE);
    return result;
}


int
second_pass(FILE *fptr, symbol_node **list)
{
    IC = 0;
    DC = 0;
    line_num = 0;


    free_list(*&list);
    return 0;
}