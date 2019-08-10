/**
 * structs and definitions for the creating of the symbol list
 * each node should consist the properties of a code line, a data line
 * or a macro definition.
 */

#ifndef SYMBOL_H
#define SYMBOL_H


enum SYMBOL
{
    SYMBOL_MACRO,
    SYMBOL_DATA_STRING,
    SYMBOL_DATA_NUMBERS,
    SYMBOL_EXTERNAL,
    SYMBOL_CODE
};

/* MACRO STRUCT */
typedef struct macro_st
{
    char *name;
    int data;
} macro_t;

/* DIRECTIVE UNION */
typedef union directive_st
{   
    int *nums;
    char *data;
} directive_t;

/* INSTRUCTION STRUCT */
typedef struct instruction_st
{
    int type;
} instruction_t;

/* SYMBOL UNION, CONTAINS ONE OF THE STRUCTS ABOVE */
union symbol_un
{
    macro_t *macro;
    directive_t *directive;
    instruction_t *instruction;
};

/* SYMBOLS STRUCT */
typedef struct symbol_st
{
    union symbol_un *symbol;
    enum SYMBOL type; /* according to enum SYMBOL */

}symbol_t;

/************************************************************/

/* SYMBOL NODE STRUCT */
typedef struct symbol_node
{
    char *name;
    int property;
    int value;
    struct symbol_node *next;
} symbol_node;


/* Functions that are visible */
symbol_t *init_symbol(enum SYMBOL type);
symbol_node *next_node(symbol_node **list, char *name, int value, enum SYMBOL property);
int search_list(const symbol_node *list, char *name);
void free_list(symbol_node **list);
void free_symbol(symbol_t *symbol);

#endif