#include <stdlib.h>
#include <string.h>
#include "misc.h"
#include "symbols.h"

symbol_t *init_macro();
symbol_t *init_data();
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
    case SYMBOL_DATA:
        sym = init_data();
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
    case SYMBOL_DATA:
        SAFE_FREE(sym->symbol->directive)
        SAFE_FREE(sym->symbol)
        SAFE_FREE(sym)
        return;
    case SYMBOL_CODE:
        /* code */
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
init_data()
{
    symbol_t *data_symbol = (symbol_t *)malloc(sizeof(symbol_t));
    union symbol_un *symbol = (union symbol_un *)malloc(sizeof(union symbol_un));
    directive_t *directive = (directive_t *)malloc(sizeof(directive_t));
    char *data =(char *)malloc(sizeof(char)*LINE_LEN);

    if(!data_symbol || !symbol || !directive || !data)
        return NULL;

    directive->data = data;
    symbol->directive = directive;
    data_symbol->symbol = symbol;

    data_symbol->type = SYMBOL_DATA;

    return data_symbol;
}


/* Initialize symbol struct for the code lines. */
symbol_t *
init_code()
{
    return NULL;
}


symbol_node *
next_node(symbol_node **list, char *name, int value, enum SYMBOL property)
{
    symbol_node *tmp = *list;

    symbol_node *node = (symbol_node *)malloc(sizeof(symbol_node));
    char *node_name = (char *)malloc(sizeof(char)*strlen(name));

    if(!node || !node_name)
        return NULL;

    /* The list is not empty; get the last node */
     if(*list)
    {
        while(tmp)
            tmp = tmp->next;
    }

    node_name = name;

    node->name = node_name;
    node->value = value;
    node->property = property;
    node->next = NULL;

    /* The list is not empty attach the new node to the tail */
    if(*list)
        (*list)->next = node;
      
    /* The list is empty, returning the new node as it's the only node in the new list */
    else
        *list = node;

    return *list;
}

int
search_list(const symbol_node *list, char *name)
{
    symbol_node **tmp = (symbol_node **) &list;
    if(!list)
        return 0;
    while((*tmp))
    {
        if(strcmp_hash((*tmp)->name, name))
            return 1;
        (*tmp) = (*tmp)->next;
    }
    return 0;
}



void 
free_list(symbol_node **list)
{
    symbol_node **tmp = *&list;
    while(*tmp)
    {
        *list = (*list)->next;
        SAFE_FREE((*tmp)->name)
        SAFE_FREE(*tmp)
        tmp = *&list;
    }
}