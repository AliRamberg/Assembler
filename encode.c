#include <stdio.h>
#include "asmbl.h"
#include "symbols.h"
#include "misc.h"
#include "line.h"


/**
 * Adds a new element to the symbol linked list.
 * The new node is added based on the symbol type detailed in the pLINE->parsed->type variable.
 * Handles already existing nodes in the list.
 * returns 0 on success or 1 on any error or warning.
 */
int
encode(enum PARSE parse, line_t *pLINE, symbol_node **list)
{
    int i, ERR_FLAG = FALSE;
    switch ((int) parse)
    {

    case PARSED_MACRO:
        if(!search_list(*list, pLINE->parsed->symbol->macro->name))
            next_node(*&list, pLINE->parsed->symbol->macro->name, pLINE->parsed->symbol->macro->data, SYMBOL_MACRO);
        else
        {
            ERROR_MSG("Macro name was already set");
            ERR_FLAG =TRUE;
        }
        break;

    case PARSED_DIRECTIVE:
        if(pLINE->parsed->type == SYMBOL_EXTERNAL)
        {
            /* .external ???  */;
        }
        next_node(*&list, EMPTY_STRING, DC, SYMBOL_DATA_STRING);
        for(i = DC; i < pLINE->len; i++)
            data_arr[DC + i].reg = pLINE->parsed->symbol->directive->data[i];
        DC += pLINE->len;
        break;
    
    case PARSED_INSTRUCTION:
        /* code */
        break;
        
    case PARSED_LABEL:
        if(!search_list(*list, pLINE->label))
        {
            next_node(*&list, pLINE->label, IC + 100, SYMBOL_CODE);
            IC++;
        }
        else
        {
            ERROR_MSG("Label name was already set")
            ERR_FLAG = TRUE;
        }
        break;
    case (PARSED_LABEL | PARSED_MACRO):
        ERROR_MSG("No label is allowed on a macro line")
        ERR_FLAG = TRUE;
        break;
    case (PARSED_LABEL | PARSED_DIRECTIVE):
        if(!search_list(*list, pLINE->label))
        {
            /* .string */
            if (pLINE->parsed->type == SYMBOL_DATA_STRING)
            {
                next_node(*&list, pLINE->label, DC, SYMBOL_DATA_STRING);
                for(i = 0; i < pLINE->len; i++)
                    data_arr[DC + i].reg = pLINE->parsed->symbol->directive->data[i];
            }
            /* .data */
            else if (pLINE->parsed->type == SYMBOL_DATA_NUMBERS)
            {
                next_node(*&list, pLINE->label, DC, SYMBOL_DATA_NUMBERS);
                for(i = 0; i < pLINE->len; i++)
                    data_arr[DC + i].reg = pLINE->parsed->symbol->directive->nums[i];
            }
            else if (pLINE->parsed->type == SYMBOL_EXTERNAL)
            {
                next_node(*&list, pLINE->label, IC + 100, SYMBOL_CODE);
                break; /* No need to update the DC */
            }
            DC += pLINE->len;
            break;                
        
        }
        else
        {
            ERROR_MSG("Label name was already set")
            ERR_FLAG = TRUE;
        }
        break;
    case (PARSED_LABEL | PARSED_INSTRUCTION):
        break;
    default:
        break;
    }
    return ERR_FLAG;
}