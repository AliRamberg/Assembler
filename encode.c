#include <stdio.h>
#include "asmbl.h"
#include "symbols.h"
#include "misc.h"
#include "line.h"
#include "opcodes.h"


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
    int value = ERROR;
    switch ((int) parse)
    {

    case PARSED_MACRO:
        if(!search_list(*list, pLINE->parsed->symbol->macro->name, NULL))
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
            next_node(*&list, EMPTY_STRING, IC + 100, SYMBOL_EXTERNAL);
            break;
        }
        next_node(*&list, EMPTY_STRING, DC, SYMBOL_DATA_STRING);
        for(i = 0; i < pLINE->len; i++)
            data_arr[DC + i].reg = pLINE->parsed->symbol->directive->data[i];
        DC += pLINE->len;
        break;
    
    case PARSED_INSTRUCTION:
        i = 1;
        /* INSTRUCTION WORD */
        instruction_arr[IC].reg = (pLINE->parsed->symbol->instruction->opcode << 6) | (pLINE->parsed->symbol->instruction->addmod_src << 4) | (pLINE->parsed->symbol->instruction->addmod_dst << 2);
        /* SOURCE WORD */
        if(pLINE->len > 1)
        {
            /* IS A NAME, so it's not a number */
            if(pLINE->parsed->symbol->instruction->are_src & (EXTERNAL | RELOCATABLE))
            {
                /* RELOCATABLE */
                if(search_list(*list, pLINE->parsed->symbol->instruction->source, &value))
                    instruction_arr[IC + i++].reg = (value << 2) | RELOCATABLE;
                /* EXTERNAL */
                else
                    instruction_arr[IC + i++].reg = (value << 2) | EXTERNAL;
            }
            /* ABSOLUTE */
            else
                instruction_arr[IC + i++].reg = (atoi(pLINE->parsed->symbol->instruction->source) << 2) | ABSOLUTE;
        }
        /* DESTINATION WORD*/
        if (pLINE->len > 0)
        {
            instruction_arr[IC + i].reg = (atoi(pLINE->parsed->symbol->instruction->destination) << 2) | (pLINE->parsed->symbol->instruction->are_dst);
        }
        for(i = 0; i < pLINE->len; i++)
            printf("%d \n", instruction_arr[IC + i].reg);
        IC += pLINE->len;
        break;
        
    case PARSED_LABEL:
        if(!search_list(*list, pLINE->label, NULL))
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
        if(!search_list(*list, pLINE->label, NULL))
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

int
conv_addmod(int mode)
{
    switch (mode)
    {
    case ADDMODE_0:
        return 0;
    case ADDMODE_1:
        return 1;    
    case ADDMODE_2:
        return 2;
    case ADDMODE_3:
        return 3;
    }
    return ERROR;
}