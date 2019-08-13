#include <stdio.h>
#include "asmbl.h"
#include "symbols.h"
#include "misc.h"
#include "line.h"
#include "opcodes.h"
#include "globals.h"


void build_absolute_word(symbol_node *list, char *name, int bSRC, int index);
void build_relocatable_word(symbol_node *list, char *name, int index);
void build_external_word(symbol_node *list, char *name, int index);
int reg_address(char *reg, int src);
void encode_inst(line_t *pLINE, symbol_node **list);


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
        if(!(search_list(*list, pLINE->parsed->symbol->macro->name, NULL, NULL) != ERROR))
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
        encode_inst(pLINE, list);
        break;
        
    case PARSED_LABEL:
        if(!(search_list(*list, pLINE->label, NULL, NULL) != ERROR))
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
        if(!(search_list(*list, pLINE->label, NULL, NULL) != ERROR))
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
        if(!(search_list(*list, pLINE->label, NULL, NULL) != ERROR))
        {
            next_node(*&list, pLINE->label, IC + 100, SYMBOL_CODE);
        }
        else
        {
            ERROR_MSG("Label name was already set")
            ERR_FLAG = TRUE;
        }
        encode_inst(pLINE, list);
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


int
entry_encode(char *entry, line_t *pLINE, symbol_node **list)
{
    next_node(*&list, entry, IC + 100, SYMBOL_ENTRY);
    return 0;
}



void
build_absolute_word(symbol_node *list, char *name, int bSRC, int index)
{
    int value;
    if (is_register(name))
    {
        if (bSRC)
        {
            /* SRC IS REGISTER */
            instruction_arr[index].reg = reg_address(name, TRUE);
            return;
        }
        /* DST IS REGISTER */
        instruction_arr[index].reg = reg_address(name, FALSE);
        return;
    }
    if((search_list(list, name, &value, NULL) != ERROR))
    {
        instruction_arr[index].reg = value << 2;
        return;
    }
    if((value = is_num(name) != _12BIT_MIN))
    {
        instruction_arr[index].reg = value << 2;
        return;
    }
    fprintf(stderr, "Failed to create absolute word\n");
    return;    
}


void
build_relocatable_word(symbol_node *list, char *name, int index)
{
    int value, property;
    if((search_list(list, name, &value, &property) != ERROR) && property != SYMBOL_EXTERNAL)
    {
        instruction_arr[index].reg = (value << 2) | RELOCATABLE;
        return;
    }
    fprintf(stderr, "Failed to find relocatable symbol\n");
    return;
}


void
build_external_word(symbol_node *list, char *name, int index)
{
    int value, property;
    if((search_list(list, name, &value, &property) != ERROR) && property == SYMBOL_EXTERNAL)
    {
        instruction_arr[index].reg = (value << 2) | EXTERNAL;
        return;
    }
    fprintf(stderr, "Failed to find external symbol\n");
    return;
}

int
reg_address(char *reg, int src)
{
    int regnum = ERROR;
    if((regnum = is_register(reg)) == ERROR)
        return ERROR;
    if(src)
        return (regnum << 4);
    return (regnum << 2);    
}

void
encode_inst(line_t *pLINE, symbol_node **list)
{
    int i = 1, src_addmode, dst_addmode, src_are, dst_are;
    char *src_name, *dst_name;
    int property, value;

    /* Addressing mode */
    src_addmode = pLINE->parsed->symbol->instruction->source->addmod;
    dst_addmode = pLINE->parsed->symbol->instruction->destination->addmod;
    /* label name or register */
    src_name = pLINE->parsed->symbol->instruction->source->op->name;
    dst_name = pLINE->parsed->symbol->instruction->destination->op->name;
    /* are property */
    src_are = pLINE->parsed->symbol->instruction->source->are;
    dst_are = pLINE->parsed->symbol->instruction->destination->are;

    /* INSTRUCTION FIRST WORD */
    instruction_arr[IC].reg = (pLINE->parsed->symbol->instruction->opcode << 6) | (conv_addmod(src_addmode) << 4) | (conv_addmod(dst_addmode) << 2);

    printf("%d\n", instruction_arr[IC].reg);

    /* The source and the destination are registers */
    if ((is_register(src_name) != ERROR) && (is_register(dst_name)) != ERROR)
    {
        instruction_arr[IC + 1].reg =  reg_address(src_name, TRUE) | reg_address(dst_name, FALSE);
        printf("%d\n", instruction_arr[IC + 1].reg);
        IC += 2;
        return;
    }
    


    /* SRC WORD */
    if(pLINE->len > 1)
    {
        if(src_are == ABSOLUTE)
            build_absolute_word(*list, src_name, TRUE, IC + i);
        else if((search_list(*list, src_name, &value, &property) != ERROR) && (property & (SYMBOL_DATA_NUMBERS | SYMBOL_DATA_STRING)))
            build_relocatable_word(*list, src_name, IC + i);
        else
            build_external_word(*list, src_name, IC + i);
        i++;
    }
    /* DST WORD */
    if(pLINE->len > 0)
    {
        if(dst_are == ABSOLUTE)
            build_absolute_word(*list, src_name, FALSE, IC + i);
        else if((search_list(*list, src_name, &value, &property) != ERROR) && (property & (SYMBOL_DATA_NUMBERS | SYMBOL_DATA_STRING)))
            build_relocatable_word(*list, dst_name, IC + i);
        else
            build_external_word(*list, dst_name, IC + i);
    }

    IC += pLINE->len;
    return;
}