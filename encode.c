#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "asmbl.h"
#include "symbols.h"
#include "misc.h"
#include "line.h"
#include "line.h"
#include "opcodes.h"
#include "globals.h"


void build_absolute_word(symbol_node *list, char *name, int opvalue, int bSRC, int index);
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
    char tmp[LABEL_LEN];
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
            
            strcpy(tmp, pLINE->label);
            next_node(*&list, tmp, IC + 100, SYMBOL_CODE);
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
    return TRUE;
}



void
build_absolute_word(symbol_node *list, char *name, int opvalue, int bSRC, int index)
{
    int value;
    if(is_valid(opvalue) && !name)
    {
        instruction_arr[index].reg = (opvalue << 2) | ABSOLUTE;
        return;
    }
    if (is_register(name) != ERROR)
    {
        if (bSRC)
        {
            /* SRC IS REGISTER */
            instruction_arr[index].reg = reg_address(name, TRUE) | ABSOLUTE;
            return;
        }
        /* DST IS REGISTER */
        instruction_arr[index].reg = reg_address(name, FALSE) | ABSOLUTE;
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
    ERROR_MSG("Failed to create absolute word")
    return;    
}


void
build_relocatable_word(symbol_node *list, char *name, int index)
{
    int value = -1, property = -1;
    search_list(list, name, &value, &property);
    if((value != ERROR) && property != SYMBOL_EXTERNAL)
    {
        instruction_arr[index].reg = (value << 2) | RELOCATABLE;
        return;
    }
    ERROR_MSG("Failed to find relocatable symbol")
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
    ERROR_MSG("Failed to find external symbol")
    return;
}

int
reg_address(char *reg, int src)
{
    int regnum = ERROR;
    if((regnum = is_register(reg)) == ERROR)
        return ERROR;
    if(src)
        return (regnum << 5);
    return (regnum << 2);    
}

void
encode_inst(line_t *pLINE, symbol_node **list)
{
    int i = 1, src_addmode, dst_addmode, src_are, dst_are, src_index = -1, dst_index = -1;
    int src_val, dst_val;
    
    char *src_name, *dst_name;
    int property, value, opcode, reg;
    src_name = (char *)calloc(sizeof(char), MACRO_LEN);
    dst_name = (char *)calloc(sizeof(char), MACRO_LEN);

    /* Addressing mode */
    src_addmode = pLINE->parsed->symbol->instruction->source->addmod;
    dst_addmode = pLINE->parsed->symbol->instruction->destination->addmod;

    /* label name or register */
    if (!is_valid(pLINE->parsed->symbol->instruction->source->op->value))
        strcpy(src_name, pLINE->parsed->symbol->instruction->source->op->name);
    if (!is_valid(pLINE->parsed->symbol->instruction->destination->op->value))
        strcpy(dst_name, pLINE->parsed->symbol->instruction->destination->op->name);

    /* numeric values */
    src_val = pLINE->parsed->symbol->instruction->source->op->value;
    dst_val = pLINE->parsed->symbol->instruction->destination->op->value;

    /* index values */
    if (!is_name(pLINE->parsed->symbol->instruction->source->str_index))
        src_index = pLINE->parsed->symbol->instruction->source->index;
    else
        src_index = is_num(pLINE->parsed->symbol->instruction->source->str_index);
    if (!is_name(pLINE->parsed->symbol->instruction->destination->str_index))
        dst_index = pLINE->parsed->symbol->instruction->destination->index;
    else
        dst_index = is_num(pLINE->parsed->symbol->instruction->destination->str_index);
    
    /* are property */
    src_are = pLINE->parsed->symbol->instruction->source->are;
    dst_are = pLINE->parsed->symbol->instruction->destination->are;

    opcode = pLINE->parsed->symbol->instruction->opcode << 6;
    reg = opcode | (conv_addmod(src_addmode) << 4) | (conv_addmod(dst_addmode) << 2);
    /* INSTRUCTION FIRST WORD */
    instruction_arr[IC].reg = reg;
    
    instruction_arr[IC].src_name = src_name;
    instruction_arr[IC].dst_name = dst_name;
    
    /* The source and the destination are registers */
    if ((is_register(src_name) != ERROR) && (is_register(dst_name) != ERROR))
    {
        instruction_arr[IC + 1].reg =  reg_address(src_name, TRUE) | reg_address(dst_name, FALSE);
        IC += 2;
        SAFE_FREE(src_name)
        SAFE_FREE(dst_name)
        return;
    }

    /* SRC WORD */
    if(pLINE->len > 2)
    {
        instruction_arr[IC].src_addmod = src_addmode;

        if(src_are == ABSOLUTE)
        {
            if(!*src_name)
                build_absolute_word(*list, NULL, src_val, TRUE, IC + i);
            else
                build_absolute_word(*list, src_name, 0, TRUE, IC + i);
        }
        else if((search_list(*list, src_name, &value, &property) != ERROR) && (property & SYMBOL_MACRO))
            build_relocatable_word(*list, src_name, IC + i);
        i++;

        if(src_addmode == ADDMODE_2)
        {
            if(src_index != -1)
                build_absolute_word(*list, NULL, src_index, TRUE, IC + i);
            else
                build_relocatable_word(*list, pLINE->parsed->symbol->instruction->source->str_index, IC + i);
            
            i++;
        }
        /* everything else will get built on the second pass */
    }
    /* DST WORD */
    if(pLINE->len > 1)
    {
        instruction_arr[IC].dst_addmod = dst_addmode;

        if(dst_are == ABSOLUTE)
        {
            if(!*dst_name)
                build_absolute_word(*list, NULL, dst_val, FALSE, IC + i);
            else
                build_absolute_word(*list, dst_name, 0, FALSE, IC + i);
        }
        /* only macros */
        else if((search_list(*list, src_name, &value, &property) != ERROR) && (property & SYMBOL_MACRO))
            build_relocatable_word(*list, dst_name, IC + i);
        i++;

        if(dst_addmode == ADDMODE_2)
        {
            if(dst_index != -1)
                build_absolute_word(*list, NULL, dst_index, TRUE, IC + i);
            else
                build_relocatable_word(*list, pLINE->parsed->symbol->instruction->destination->str_index, IC + i);
            
            i++;
        }
        /* everything else will get built on the second pass */
        
    }
    instruction_arr[IC].len = pLINE->len;
    IC += pLINE->len;
    return;
}


void
complete_encoding(symbol_node *list, int start, int oIC)
{
    int i, j, src_addmode, dst_addmode;
    char *src, *dst;
       
    for(i = start; i < oIC; i++)
    {
        int property;
        src = instruction_arr[i].src_name;
        dst = instruction_arr[i].dst_name;
        src_addmode = instruction_arr[i].src_addmod;
        dst_addmode = instruction_arr[i].dst_addmod;

        for(j = 0; j < instruction_arr[i].len; j++);


        if (!strcmp_hash(dst, EMPTY_STRING) && !(is_register(dst) != ERROR) && search_list(list, dst, 0, &property) && property != SYMBOL_MACRO)
        {
            if (!strcmp_hash(src, EMPTY_STRING) && search_list(list, src, 0, &property) && property != SYMBOL_MACRO && !(is_register(src) != ERROR))
            {
                build_relocatable_word(list, src, i + 1);
                if(src_addmode == ADDMODE_2) i++;
                i++;
            }
            if((is_register(src) != ERROR)) i++;
            if(dst_addmode == ADDMODE_2) i++;
            build_relocatable_word(list, dst, i + 1);
            i++;
        }
    }

    return;
}
