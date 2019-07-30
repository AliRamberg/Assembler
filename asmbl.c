#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "asmbl.h"
#include "pass.h"
#include "misc.h"

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
    if (argc < 2)
    {
        fprintf(stderr, "No input files\nAssembling terminated.\n");
        return NO_FILE;
    }

    while(--argc)
    {
        fprintf(stdout, "Assembling file %s...\n", argv[argc]);
        fptr = fopen(argv[argc], "r");
        if(!fptr)
        {
            fprintf(stderr, "Failed to open file %s, %d\n", argv[argc], errno);
            continue;
        }
        pass_return = first_pass(fptr);
        if (pass_return)
        {
            fprintf(stderr, "First pass failed! (%s)\nTerminating... %d\n",argv[argc], 1);
            fclose(fptr);
            continue;
        }
        pass_return = second_pass(fptr);
        if (pass_return)
        {
            fprintf(stderr, "Second pass failed! (%s)\nTerminating... %d\n",argv[argc], 1);
            fclose(fptr);
            continue;
        }
        fclose(fptr);
        fprintf(stdout, "Fishished assembling file %s\n", argv[argc]);
    }
    /* pass_return = insert_protected("ABC");
    if(pass_return)
        printf("%d - BAD\n", pass_return);
    else
        printf("%d - GOOD\n", pass_return);
    printf("Protected element is %ld\n.", protected[0]); */
    return EXIT_SUCCESS;
}
