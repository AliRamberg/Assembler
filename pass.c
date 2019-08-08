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
    char *line = (char *)malloc(sizeof(char) * LINE_LEN);
    line_t *pLINE = (line_t *)malloc(sizeof(line_t));
    int parse;

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
            ERROR_MSG("Line Exceeds max line length.")
            return EXCEEDS_MAX_LENGTH;
        }

        /* Check if line is a whitespace or a comment */
        if(skipable_line(pLINE->line))
            continue;


        /***************** PARSING LINE *****************/
        parse = parse_line(pLINE);                    /**/
        /************************************************/            

        switch (parse)
        {

        case PARSED_MACRO:
            next_node(
                *&list,
                pLINE->parsed->symbol->macro->name,
                pLINE->parsed->symbol->macro->data,
                SYMBOL_MACRO
            );
            IC += 2;
            break;

        case PARSED_DIRECTIVE:
            next_node(*&list, EMPTY_STRING, IC + 100, SYMBOL_DATA);
            IC += pLINE->len;
            break;
            
        case PARSED_INSTRUCTION:
            /* code */
            break;
            
        case PARSED_LABEL:
            if(search_list(*list, pLINE->label))
            {
                ERROR_MSG("Label was already set\nAborting...\n")
                return 1;
            }
            next_node(*&list, pLINE->label, IC + 100, SYMBOL_CODE);
            IC++;
            break;
        case (PARSED_LABEL | PARSED_MACRO):
            ERROR_MSG("No label is allowed on a macro line\nAborting...\n")
            return 1;
        case (PARSED_LABEL | PARSED_DIRECTIVE):
            if(search_list(*list, pLINE->label))
            {
                ERROR_MSG("Label was already set\nAborting...\n")
                return 1;
            }
            next_node(*&list, pLINE->label, IC + 100, SYMBOL_DATA);
            IC += pLINE->len;
            break;
        case (PARSED_LABEL | PARSED_INSTRUCTION):
            break;
        default:
            ERROR_MSG("Unrecognized statement\nAborting...\n")
            return 1;
        }
    }
    LINE_FREE(pLINE);
    return 0;
}


int
second_pass(FILE *fptr, symbol_node **list)
{
    IC = 0;
    line_num = 0;


    free_list(*&list);
    return 0;
}