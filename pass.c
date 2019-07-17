#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "pass.h"
#include "asmbl.h"
#include "macro.h"
#include "line.h"

int
first_pass(FILE *fptr)
{
    char *line = (char *)malloc(sizeof(char) * LINE_LEN);
    line_t *pLINE = (line_t *)malloc(sizeof(line_t));
    /* int args; */
    /* Reading line by line of the file, and line is not whitespace or a comment */
    while ((pLINE->line = fgets(line, LINE_LEN, fptr)) != NULL && (pLINE->line[0] != ';' || !isprint(line[0])))
    {
        /* args = parse_line(pLINE); */

        /* Is MACRO? */
        
        if((pLINE->label = is_label(line)) != NULL)
            fprintf(stdout, "THIS IS LABEL: '%s'\n", pLINE->label);
        else
            fprintf(stdout, "THIS IS NOT LABEL: '%5s'\n", pLINE->line);
        /* Is Data Holder? such as array or variable? */
        /** 
         * TODO - Configure array parser
         */
        /* .extern or .entry ? */
        /**
         * TODO - Configure .extern/.entry
         */
        
        /* fprintf(stdout, "%s", pLINE->line); */

    }
    safe_free(line);
    return EXIT_SUCCESS;
}


int
second_pass(FILE *fptr)
{
    return EXIT_SUCCESS;
}