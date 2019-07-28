#include "line.h"

typedef struct symbol_st
{
    union _symbol_un
    {
        struct macro_st
        {
            char *name;
            char *data;
        } *macro_st;
        struct directive_st
        {
            char *directive;
        } *directive_st;
        struct instruction_st
        {
            char inst;
        } *instruction_st;
    } *symbol_un;
    line_t *line_t;
}symbol_t;

typedef union _symbol_un symbol_un;

