#include "line.h"

typedef struct symbol_st
{
    union _symbol_un
    {
        struct macro_st
        {
            char *macro;
        }macro_st;
        struct directive_st
        {
            char *directive;
        }directive_st;
        struct instruction_st
        {
            char inst;
        }instruction_st;
    } symbol_un;
    char *label;
};

typedef union _symbol_un symbol_un;

