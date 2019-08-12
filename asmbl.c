#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "asmbl.h"
#include "pass.h"
#include "misc.h"
#include "files.h"

/* 
static REGISTER r0, r1, r2, r3, r4, r5, r6, r7;
*/

/*
 * ─── START HERE ─────────────────────────────────────────────────────────────────
 */
int
main(int argc, char const *argv[])
{
    int pass_return;
    FILE *fptr;     /* pointer to file object */
    symbol_node *list = NULL;


    if (argc < 2)
    {
        fprintf(stderr, "No input files\nAssembling terminated.\n");
        return NO_FILE;
    }


    while(--argc)
    {
        const char *filename = argv[argc];

        if(!validate_filename(filename))
        {
            fprintf(stderr, "The assembler accepts only .as files\n");
            continue;
        }

        fprintf(stdout, "Assembling file %s...\n", filename);

        fptr = fopen(filename, "r");
        if(!fptr)
        {
            fprintf(stderr, "Failed to open file %s, %d\n", filename, errno);
            continue;
        }
        pass_return = first_pass(fptr, &list);
        if (pass_return)
        {
            fprintf(stderr, "First pass failed! (%s)\nTerminating... %d\n",filename, 1);
            fclose(fptr);
            continue;
        }
        pass_return = second_pass(fptr, &list);
        if (pass_return)
        {
            fprintf(stderr, "Second pass failed! (%s)\nTerminating... %d\n",filename, 1);
            fclose(fptr);
            continue;
        }
        fclose(fptr);
        fprintf(stdout, "Fishished assembling file %s\n", filename);
    }
    return EXIT_SUCCESS;
}
