#include <string.h>
#include <ctype.h>
#include <assert.h> /* DEBUG */
#include "line.h"
#include "asmbl.h"
#include "misc.h"
#include "symbols.h"
#include "opcodes.h"
#include "globals.h"


char *strsub(char *, size_t , char *);
char *is_label(char *);
int is_macro(line_t *);
int is_directive(line_t *);
int is_instruction(line_t *);

enum PARSE 
parse_line(line_t *pLINE)
{
    char *label;
    int res = NOT_PARSED;

    /* Trim whitespaces */
    clear_str(pLINE->line);

    /********************************************\
                        LABELS
    \********************************************/
    if((label = is_label(pLINE->line)))
    {
        res |= PARSED_LABEL;
        pLINE->label = label;

        /* strtok label from line, thus taking only the substring */
        strtok(pLINE->line, " \t");
        /* pLINE->line += strlen(pLINE->label) + 1; */
        pLINE->line = strtok(NULL, "\0");
    }
    else
        pLINE->label = NULL;
    /********************************************/

    /********************************************\
                        MACROS                    
    \********************************************/
    if(is_macro(pLINE))
        return (res | PARSED_MACRO);
    /********************************************/

    /********************************************\
                     DIRECTIVES                   
    \********************************************/
    if (is_directive(pLINE))
        return (res | PARSED_DIRECTIVE);
    /********************************************/

    /********************************************\
                     INSTRUCTION                   
    \********************************************/
    if (is_instruction(pLINE))
        return (res | PARSED_INSTRUCTION);
    /********************************************/
    
    return res;
}


int
is_directive(line_t *pLINE)
{
    char *type, *ch, *data, *tmp;
    symbol_t *directive;
    
    tmp = (char *)malloc(sizeof(char)*LINE_LEN);
    strcpy(tmp, pLINE->line);

    ch = tmp;
    data = (char *)malloc(sizeof(char)*LINE_LEN);
    

    /* trim whitespaces from start */
    while(isspace(*ch++));

    if(*(--ch) != '.')
        return FALSE;
    
    type = strtok(ch,  " ");
    data = strtok(NULL, "\0");

    /***************************************************************************************/
    /**************************************** .data ****************************************/
    /***************************************************************************************/
    if(strcmp_hash(type, ".data"))
    {
        int i = 0;
        int num;
        char *tmp, tmp_data[LINE_LEN];
        strcpy(tmp_data, data);
        tmp = strtok(tmp_data, ",");
        if(!(directive = init_symbol(SYMBOL_DATA_NUMBERS)))
        {
            ERROR_MSG("Failed to allocate memory for symbol_t")
            return FALSE;
        }
        
        while(tmp)
        {
            if((num = is_num(tmp)) != _12BIT_MIN)
                directive->symbol->directive->nums[i++] = num;
            tmp = strtok(NULL, ",");
        }
        pLINE->len = i;
        pLINE->parsed = directive;
        return TRUE;
    }
    /***************************************************************************************/

    /***************************************************************************************/
    /*************************************** .string ***************************************/
    /***************************************************************************************/
    else if (strcmp_hash(type, ".string"))
    {
        if(data[0] == '"' && data[strlen(data) - 1] == '"')
        {
            if((directive = init_symbol(SYMBOL_DATA_STRING)))
            {
                /* Clear quotation marks */
                data++;
                data[strlen(data) - 1] = '\0';
                
                /* Validates the characters in the string are of the ascii family */
                if(!is_string(data))
                        ERROR_MSG("The assembler doesn't support non-ascii characters")

                directive->symbol->directive->data = data;
                pLINE->len = strlen(data) + 1 ;
                pLINE->parsed = directive;
                return TRUE;
            }
            else
            {
                ERROR_MSG("Failed to allocate memory for symbol_t")
                return FALSE;
            }
        }
        else
        {
            ERROR_MSG("Data is not enclosed with quatation marks")
            return FALSE;
        }        
    }
    /***************************************************************************************/
    /*   
    else if (strcmp_hash(type, ".entry"))
        {
            DO NOTHING IF FOUND .entry
        } 
    */
    else if (strcmp_hash(type, ".extern"))
    {
        char *tmp, tmp_data1[LINE_LEN];
        int len = 0;
        trim_white(data);
        strcpy(tmp_data1, data);

        tmp = strtok(tmp_data1, ",");
        while(tmp)
        {
            if(is_string(tmp) && isdigit(tmp[0]))
            {
                tmp = strtok(NULL, ",");
                len++;
            }
            else
            {
                ERROR_MSG("Illegal symbol name")
                return FALSE;
            }
            
        }

        if((directive = init_symbol(SYMBOL_DATA_STRING)))
        {
            directive->symbol->directive->data = data;
            pLINE->parsed = directive;
            pLINE->parsed->type = SYMBOL_EXTERNAL;
            pLINE->len = len;
            return TRUE;
        }
        else
            return FALSE;         
    }
    
    SAFE_FREE(type)
    SAFE_FREE(data)
    

    return FALSE;
}

int
is_instruction(line_t *pLINE)
{
    char *tmp, *pst;
    char *operand, *line, *operand2;
    int code = ILLEGAL_INST;
    tmp = pst = (char *)malloc(sizeof(char)*LINE_LEN);
    if(tmp)
        strcpy(tmp, pLINE->line);
    else
        return FALSE;

    /* Extracts the opcode name */
    while(isspace(*tmp))
        tmp++;
    strtok(tmp, " \t");

    /* Validates the opcodes name */
    if((code = is_opcode(tmp)) != ILLEGAL_INST)
    {
        size_t sz = 1; /* number of words, atleast one for the main instruction */
        int addmod, abs = ERROR;
        char macro_name[MACRO_LEN];
        symbol_t *instruction = init_symbol(SYMBOL_CODE);
        int op = check_operands(pLINE->line, code);
        if(op == ERROR || !instruction)
        {
            SAFE_FREE(pst)
            free_symbol(instruction);
            return FALSE;
        }

        line = clear_str(pLINE->line);
        strtok(line, "\t ");    /* remove the opcode */
        operand = strtok(NULL, "\t ,");
        if(operand)
        {
            operand2 = operand + strlen(operand) + 1;
            while(isspace(*operand2) || *operand2 == ',') operand2++;
        }
        /**************************************************************************/
        if (op > 1)
        {
            char name[MACRO_LEN];
            char index[MACRO_LEN];
            /* Get the correct address mode for the source operand */
            addmod = get_addmode(operand, code, SRC, &abs, &*macro_name);

            if(addmod == ERROR)
            {
                SAFE_FREE(pst)
                return FALSE;
            }

            switch (addmod)
            {
            case ADDMODE_3:
                /* Registry operand */
                instruction->symbol->instruction->source->addmod = ADDMODE_3;
                instruction->symbol->instruction->source->op->name = operand;
                instruction->symbol->instruction->source->are = ABSOLUTE;
                break;
            case ADDMODE_2:
                strcpy(name, strtok(operand, "["));
                strcpy(index, strtok(NULL, "]"));
                instruction->symbol->instruction->source->addmod = ADDMODE_2;
                instruction->symbol->instruction->source->op->name = name;
                if(is_num(index))
                    instruction->symbol->instruction->source->index = atoi(index);
                else
                    instruction->symbol->instruction->source->str_index = index;
                instruction->symbol->instruction->source->are = -1;
                break;
            case ADDMODE_1:
                instruction->symbol->instruction->source->op->name = operand;
                instruction->symbol->instruction->source->addmod = ADDMODE_1;
                instruction->symbol->instruction->source->are = -1;
                break;
            case ADDMODE_0:
                instruction->symbol->instruction->source->addmod = ADDMODE_0;
                operand++; /* Skip the '#' character */
                if((is_num(operand) != _12BIT_MIN))
                    instruction->symbol->instruction->source->op->value = atoi(operand);
                else
                    instruction->symbol->instruction->source->op->name = operand;
                instruction->symbol->instruction->source->are = ABSOLUTE;
                break;
            default:
                break;
            }

            
            sz += addmod_sz(addmod);
        }
        if(op > 0)
        {
            char name[MACRO_LEN];
            char index[MACRO_LEN];
            /* Get the correct address mode for the destination operand */
            addmod = get_addmode(operand2, code, SRC, &abs, &*macro_name);

            if(addmod == ERROR)
            {
                SAFE_FREE(pst)
                return FALSE;
            }

            switch (addmod)
            {
            case ADDMODE_3:
                /* Registry operand */
                instruction->symbol->instruction->destination->addmod = ADDMODE_3;
                instruction->symbol->instruction->destination->op->name = operand2;
                instruction->symbol->instruction->destination->are = ABSOLUTE;
                break;
            case ADDMODE_2:
                strcpy(name, strtok(operand2, "["));
                strcpy(index, strtok(NULL, "]"));
                instruction->symbol->instruction->destination->addmod = ADDMODE_2;
                instruction->symbol->instruction->destination->op->name = name;
                if(is_num(index))
                    instruction->symbol->instruction->destination->index = atoi(index);
                else
                    instruction->symbol->instruction->destination->str_index = index;
                instruction->symbol->instruction->destination->are = -1;
                break;
            case ADDMODE_1:
                instruction->symbol->instruction->destination->op->name = operand2;
                instruction->symbol->instruction->destination->addmod = ADDMODE_1;
                instruction->symbol->instruction->destination->are = -1;
                break;
            case ADDMODE_0:
                instruction->symbol->instruction->destination->addmod = ADDMODE_0;
                operand2++; /* Skip the '#' character */
                if((is_num(operand2) != _12BIT_MIN))
                    instruction->symbol->instruction->destination->op->value = atoi(operand2);
                else
                    instruction->symbol->instruction->destination->op->name = operand2;
                instruction->symbol->instruction->destination->are = ABSOLUTE;
            
            default:
                break;
            }

            
            sz += addmod_sz(addmod);
            
        }


        /* number of words */
        pLINE->len = sz;
        pLINE->parsed = instruction;
        pLINE->parsed->type = SYMBOL_CODE;
        pLINE->parsed->symbol->instruction->opcode = code;

        SAFE_FREE(pst)

        return TRUE;
    }
    ERROR_MSG("Unrecognized instruction")
    return FALSE;
}

/**
 * Extractes .define macro to its name and data.
 * Constructs a new symbol_t struct
 * returns TRUE on success or FALSE on failure.
 */
int 
is_macro(line_t *pLINE)
{
    char tmp[LINE_LEN], *ch, *name, *data_s;
    int data;

    strcpy(tmp, pLINE->line);
    ch = tmp;
    
    /* trim whitespaces from start */
    while(isspace(*ch++));

    /* Extracte .define if exists */
    ch = strtok(ch-1, " ");
    if(strcmp_hash(ch, ".define"))
    {
        symbol_t *macro = init_symbol(SYMBOL_MACRO);
        name = strtok(NULL, "=");
        if (!macro)
        {
            ERROR_MSG("Failed to allocate memory for symbol_t")
            return FALSE;
        }
        
        /* Extracting the name of the macro */
        if((name = clear_str(name)) != NULL)
            strcpy(macro->symbol->macro->name, name);
        else
        {
            ERROR_MSG("Error Extracting macro name")
            free_symbol(macro);
            return FALSE;
        }

        /* Extracting the data of the macro */
        data_s = clear_str(strtok(NULL, "\0")); 
        data = is_num(data_s);
        if(data != _12BIT_MIN)
            macro->symbol->macro->data = data;
        else
        {
            ERROR_MSG("Error extracting macro data")
            free_symbol(macro);
            return FALSE;
        }

        /* Pointing the line object to the macro object */
        pLINE->parsed = macro;
        return TRUE;
    }
    return FALSE;
}

/**
 * Check if line contains a label, returns the label name if exists.
 */
char *
is_label(char *line)
{
    char *st, *ch, *label;
    ch  = line;
    while(isspace(*ch++));
    st = ch - 1;
    if(isalpha(*st))
    {
        /* Iterate over the characters of the line until reaching the end of the label title */
        while(isgraph(*ch) && *ch  != ':')
            ch++;
        /* Reached the end of the label title */
        if(*ch == ':' && isspace(*(ch + 1)))
        {
            label = strsub(st, ch - st, line);

            /* Label is too long */
            if(ch - st > LABEL_LEN)
            {
                ERROR_MSG("Label is too long.")
                SAFE_FREE(label);
                return NULL;
            }
            /* Label is a reserved word */
            if(is_reserved(label))
            {
                ERROR_MSG("Label name is a reserved word")
                SAFE_FREE(label);
                return NULL;
            }
            return label;
        }
    }
    return NULL;
}

/* Is the line completely blank? */
int
is_whitespace(char *line)
{
    while(*line != '\0')
    {
        if(!isspace(*line++))
            return FALSE;
    }
    return TRUE;
}

/* Is the line a comment? */
int
is_comment(char *line)
{
    return (*line == ';');
}

/* Is the line worth parsing at all? */
int 
skipable_line(char *line)
{
    return (is_comment(line) || is_whitespace(line));
}

/**
 * Allocates a new string which contains the characters
 * from pointer of index pos up to len characters, 
 * adds the null terminator at the end of sub.
 * Returns the new substring; on failure returns NULL.
 * NOTE: The function uses dynamically allocated memory,
 * needs to be free()'ed with FREE_SAFE or LINE_FREE()
 */
char *
strsub(char *pos, size_t len, char *str)
{
    int c = 0;
    char *sub = (char *)malloc(sizeof(char) * len);
    if(!sub)
        return NULL;
    while(c < len)
    {
        sub[c] = *pos++;
        c++;
    }
    sub[c] = '\0';

    return sub;
}

/**
 * Check if the current line is .data .extern or .string
 */
int
skip_lines_sec_pass(line_t *pLINE)
{
    if(is_label(pLINE->line))
    {
        /* strtok label from line, thus taking only the substring */
        strtok(pLINE->line, " \t");
        /* pLINE->line += strlen(pLINE->label) + 1; */
        pLINE->line = strtok(NULL, "\0");
    }
    if(strncmp(pLINE->line, ".data", 5) == 0)
        return TRUE;
    if(strncmp(pLINE->line, ".string", 7) == 0)
        return TRUE;
    if(strncmp(pLINE->line, ".extern", 7) == 0)
        return TRUE;
    else
        return FALSE;
}

/** 
 * Check if current line is .entry directive
 */
int
is_entry(line_t *pLINE)
{
    char tmp[LINE_LEN];
    char *entry;
    strcpy(tmp, pLINE->line);
    strtok(tmp, " \t");
    if(strcmp_hash(tmp, ".entry"))
    {
        entry = strtok(NULL, " \t \0");
        if(is_name(entry))
        ;
    }
    return FALSE;

}
