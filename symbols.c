#include <stdlib.h>
#include <string.h>
#include "misc.h"
#include "symbols.h"

symbol_t *init_macro();
symbol_t *init_data(enum SYMBOL type);
symbol_t *init_code();

symbol_t *
init_symbol(enum SYMBOL type)
{
    symbol_t *sym;
    switch (type)
    {
    case SYMBOL_MACRO:
        sym = init_macro();
        if(sym)
            return sym;
        break;
    case SYMBOL_DATA_STRING:
        sym = init_data(SYMBOL_DATA_STRING);
        if(sym)
            return sym;
        break;
    case SYMBOL_DATA_NUMBERS:
        sym = init_data(SYMBOL_DATA_NUMBERS);
            if(sym)
            return sym;
        break;
    case SYMBOL_CODE:
        sym = init_code();
        if(sym)
            return sym;
        break;
    default:
        break;
    }
    return NULL;
}

void
free_symbol(symbol_t *sym)
{
    if(!sym)
        return;
    switch (sym->type)
    {
    case SYMBOL_MACRO:
        SAFE_FREE(sym->symbol->macro->name)
        SAFE_FREE(sym->symbol->macro)
        SAFE_FREE(sym->symbol)
        SAFE_FREE(sym)
        return;
    case SYMBOL_DATA_STRING:
        SAFE_FREE(sym->symbol->directive)
        SAFE_FREE(sym->symbol)
        SAFE_FREE(sym)
        return;
    case SYMBOL_DATA_NUMBERS:
        SAFE_FREE(sym->symbol->directive->nums)
        SAFE_FREE(sym->symbol->directive)
        SAFE_FREE(sym->symbol)
        SAFE_FREE(sym)
        return;
    case SYMBOL_CODE:
        SAFE_FREE(sym->symbol->instruction)
        SAFE_FREE(sym->symbol)
        SAFE_FREE(sym)
        return;
    default:
        return;
    }
}


/* Initialize symbol struct for the macro lines. */
symbol_t *
init_macro()
{
    symbol_t *macro_symbol = (symbol_t *)malloc(sizeof(symbol_t));
    union symbol_un *symbol = (union symbol_un *)malloc(sizeof(union symbol_un));
    macro_t *macro = (macro_t *)malloc(sizeof(macro_t));
    char *name = (char *)malloc(sizeof(char)*MACRO_LEN);

    if (!macro_symbol || !symbol || !macro || !name)
        return NULL;
    
    macro->name = name;
    symbol->macro = macro;
    macro_symbol->symbol = symbol;

    macro_symbol->type = SYMBOL_MACRO;

    return macro_symbol;
}

/* Initialize symbol struct for the data lines. */
symbol_t *
init_data(enum SYMBOL type)
{
    symbol_t *data_symbol = (symbol_t *)malloc(sizeof(symbol_t));
    union symbol_un *symbol = (union symbol_un *)malloc(sizeof(union symbol_un));
    directive_t *directive = (directive_t *)malloc(sizeof(directive_t));
    char *data = NULL;
    int *nums = NULL;

    if(data_symbol && symbol && directive)
    {

        /* Connecting the data objects together */
        symbol->directive = directive;
        data_symbol->symbol = symbol;

        /* for .string */
        if(type == SYMBOL_DATA_STRING)
        {
            data =(char *)malloc(sizeof(char)*LINE_LEN);
            if(data)
            {
                directive->data = data;
                data_symbol->type = SYMBOL_DATA_STRING;
                return data_symbol;
            }
            return NULL;
        }
        /* for .data */
        if (type == SYMBOL_DATA_NUMBERS)
        {
            nums = (int *)malloc(sizeof(int)*LINE_LEN);
            if(nums)
            {
                directive->nums = nums;
                data_symbol->type = SYMBOL_DATA_NUMBERS;
                return data_symbol;
            }
            return NULL;
        }
    }
    
    SAFE_FREE(data_symbol)
    SAFE_FREE(data)
    SAFE_FREE(directive)
    return NULL;
        
}


/* Initialize symbol struct for the code lines. */
symbol_t *
init_code()
{
    symbol_t *data_symbol = (symbol_t *)malloc(sizeof(symbol_t));
    union symbol_un *symbol = (union symbol_un *)malloc(sizeof(union symbol_un));
    instruction_t *code = (instruction_t *)malloc(sizeof(instruction_t));

    if(!data_symbol || !symbol || !code)
    {
        SAFE_FREE(data_symbol)
        SAFE_FREE(symbol)
        SAFE_FREE(code)
        return NULL;
    }
    data_symbol->symbol = symbol;
    symbol->instruction = code;
    return data_symbol;
}


symbol_node *
next_node(symbol_node **list, char *name, int value, enum SYMBOL property)
{
    symbol_node *tmp = *list;

    symbol_node *node = (symbol_node *)malloc(sizeof(symbol_node));
    char *node_name = (char *)malloc(sizeof(char)*strlen(name));

    if(!node || !node_name)
        return NULL;

    node_name = name;
    node->name = node_name;
    node->value = value;
    node->property = property;
    node->next = NULL;

    if(!*list)
        *list = node;
    

    /* The list is not empty; get the last node andn set it */
    while(tmp)
    {
        /* reached the end of the linked list */
        if(!tmp->next)
        {
            tmp->next = node; /* setting the new node */
            break;
        }
        else
            tmp = tmp->next; /* proceeds to the next node */
    }        

    return *list;
}

int
search_list(const symbol_node *list, char *name, int *value)
{
    symbol_node **tmp = (symbol_node **) &list;
    if(!list)
        return 0;
    while((*tmp))
    {
        if(strcmp_hash((*tmp)->name, name))
            return (*tmp)->value;
        (*tmp) = (*tmp)->next;
    }
    return 0;
}



void 
free_list(symbol_node **list)
{
    symbol_node *tmp = *list;
    while(tmp)
    {
        *list = (*list)->next;
        if(tmp->name)
            SAFE_FREE(tmp->name)
        SAFE_FREE(tmp)
        tmp = *list;
    }
}