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
        SAFE_FREE(oLINE->label);
    SAFE_FREE(oLINE);
}

/* Check if str is a legal number. Accepts '+' and '-' signs */
int
is_num(char *str)
{
    char sign = str[0];
    int num, i, size = strlen(str);
    for(i = 1; i < size && isdigit(str[i]); i++);
    /* The string contain a non-digit character thus it is not a number */
    if(i < size)
        return _12BIT_MIN;
    switch (sign)
    {
    case '-':
        num = atoi(++str) * -1;
        break;
    case '+':
        num = atoi(++str);
        break;
    default:
        num = atoi(++str);
        break;
    }
    if(is_valid(num))
        return num;
    return _12BIT_MIN;
}

/* Check if string is legal */
int
is_string(char *str)
{
    char c;
    for(c = 0; c < strlen(str); c++)
        if(!ISASCII(*str++))
            return 1;
    return 0;
}