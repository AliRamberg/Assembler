#include <stdio.h>
#include <string.h>
#include "files.h"
#include "opcodes.h"
#include "asmbl.h"
#include "globals.h"


/**
 * Validates the file extention is .as
 */
int
validate_filename(const char *filename)
{
    char* ext, file[30];
    if(!filename)
        return ERROR;
    strcpy(file, filename);
    strtok(file, ".");
    ext = strtok(NULL, "\0");
    if(strcmp_hash(ext, "as"))
        return TRUE;
    return FALSE;
}

/**
 * Create new file that has ext extention
 */
FILE *
create_ext(char *filename, char *ext)
{
    if(!filename || !ext)
        return NULL;
    strtok(filename, ".");
    strcat(filename, ext);
    return fopen(filename, "w");
}

/**
 * Convert integer value to its encoded 4 bit base.
 */
void
convertbits(FILE * const filename, int bin)
{
    unsigned mask = 4;
    char sym;
    if(!filename)
        return;
    while(bin)
    {
        sym = bin & mask;
        switch (sym)
        {
        case 0:
            fprintf(filename, "%c", '*');
            break;
        case 1:
            fprintf(filename, "%c", '#');
            break;
        case 2:
            fprintf(filename, "%c", '%');
            break;
        case 3:
            fprintf(filename, "%c", '!');
            break;
        default:
            break;
        }
    bin >>= 2;
    }
}

/**
 * main memory image function printer
 */
void
fout(FILE *const filename)
{
    int i;
    if(!filename)
        return;
    for (i = 0; i < IC; i++)
    {
        fprintf(filename, "%04d\t", i+100);
        convertbits(filename, instruction_arr[i].reg);
        fprintf(filename, "\n");
    }
    fclose(filename);
}

void
fout_entext(char *filename, FILE *pext, FILE *pent)
{
    size_t i;
    int ext = FALSE, ent = FALSE;
    if(!filename || !pext || !pent)
        return;
    for (i = 0; i < IC; i++)
    {
        if(instruction_arr[i].type == SYMBOL_EXTERNAL)
        {
            fprintf(pext, "%0ld\t%s", i, instruction_arr[i].name);
            ext = TRUE;
        }
        if(instruction_arr[i].type == SYMBOL_ENTRY)
        {
            fprintf(pent, "%0ld\t%s", i, instruction_arr[i].name);
            ent = TRUE;
        }
    }
    if(!ext)
        remove(strcat(filename, ".ext"));
    if(!ent)
        remove(strcat(filename, ".ent"));
    fclose(pext);
    fclose(pent);
    return;
}