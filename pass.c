#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "pass.h"
#include "asmbl.h"
#include "misc.h"
#include "line.h"
#include "globals.h"



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
            char c;
            ERROR_MSG("Line Exceeds max line length.")
            while((c = fgetc(fptr)) != '\n' && c != '\0');
            continue;
        }

        /* Check if line is a whitespace or a comment */
        if(skipable_line(pLINE->line))
            continue;


        /***************** PARSING LINE *****************/
        parse = parse_line(pLINE);                    /**/
        /************************************************/            
        result = encode(parse, pLINE, list);

        /* Update the value of all the data symbols by IC+100 */
        
    }
    update_data(*list);
    LINE_FREE(pLINE);
    return result;
}


int
second_pass(FILE *fptr, symbol_node **list)
{
    char *line = (char *)malloc(sizeof(char)*LINE_LEN);
    line_t *pLINE = (line_t *)malloc(sizeof(line_t));;
    IC = 0;
    line_num = 0;
    
    if(!line || !pLINE)
    {
        SAFE_FREE(line)
        SAFE_FREE(pLINE)
        return FALSE;
    }
    pLINE->line = line;

    /* rewind the file */
    rewind(fptr);

    while (fgets(line, LINE_LEN + 2, fptr) != NULL)
    {
        pLINE->line = line;
        if(line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        line_num++;

        clear_str(pLINE->line);
        if(skip_lines_sec_pass(pLINE))
            continue;
        if((is_entry(pLINE)))
        {
            continue;
        }
        
        /* encode_operands(pLINE,  ) */
    }

    free_list(*&list);
    return 0;
}