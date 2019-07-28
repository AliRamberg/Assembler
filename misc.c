#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "line.h"
#include "misc.h"
#include "asmbl.h"

/* Free the line object and its dynamically allocated elements */
void
LINE_FREE(line_t *oLINE)
{
    if(oLINE->label)
        SAFE_FREE(oLINE->label)
    SAFE_FREE(oLINE)
}

/**
 * Check if str is a legal number. Accepts '+' and '-' signs
 * returns _12BIT_MIN on illegal string.
 */
int
is_num(char *str)
{
    char *ch;
    int num = (int) strtol(str, &ch,DECIMAL_BACE);
    
    if((*ch == '\0') && is_valid(num))
        return num;
    return _12BIT_MIN;
}

/**
 * Check if string is legal.
 * Contains only ASCII characters without the space character.
 */
int
is_string(char *str)
{
    char c;
    for(c = *str; c < strlen(str); c++)
        if(!IS_ASCII(c) || c == ' ')
            return 1;
    return 0;
}

/* Checks if str string is reserved and can be used */
int
is_reserved(char *str)
{
    int i;
    char *reserved_opcodes[OPCODE_NUM] = 
    {
        "mov", "cmp", "add", "sub",
        "not", "clr", "lea", "inc",
        "dec", "jmp", "bne", "red",
        "prn", "jst", "rts", "stop"
    };
    for(i = 0; i < OPCODE_NUM; i++)
    {
        if(strcmp_hash(str, reserved_opcodes[i]))
            return 1;
    }
    return 0;
}

/* Trim whitespaces from end of string */
char *
trim_white(char *str)
{
    char *pos;
    int len = strlen(str);
    if(len == 0)
        return str;
    pos = str + len - 1;
    while(pos >= str && isspace(*pos))
    {
        *pos = '\0';
        pos--;
    }
    return str;
}

/*
int
insert_protected(char *str)
{
    int i, arr_size = sizeof(protected) / sizeof(unsigned long);
    void *alloc;

    for(i = 0; i < arr_size; i++)
    {
        if(!protected[i])
        {
            protected[i] = hash(str);
            return 1;
        }
    }
    alloc = realloc(protected, arr_size + INIT_PROTECTED_SIZE);
    if(!alloc)
    {
        fprintf(stderr, "Failed to allocate new memory for protected words array.\n");
        return -1;
    }
    protected[i] = hash(str);

    return 0;
}
*/