#ifndef LINE_H
#define LINE_H

#include "symbols.h"

#define LINE_LEN 80       /* Line maximum length */
#define LABEL_LEN 31      /* Label maximum length */
#define MACRO_LEN 20      /* macro maximum length - length of ".define" */

typedef struct line_st
{
    char *line;
    char *label;
    int len; /* number of words required */
    symbol_t *parsed;
} line_t;

enum DIRECTIVE 
{
    DIRECTIVE_DATA, 
    DIRECTIVE_STRING, 
    DIRECTIVE_ENTRY, 
    DIRECTIVE_EXTERN
};

/* Three results that are available to parse_line() */
enum PARSE
{
    NOT_PARSED = 0,
    PARSED_MACRO = 1,
    PARSED_DIRECTIVE = 2,
    PARSED_INSTRUCTION = 4,
    PARSED_LABEL = 8,
    PARSED_FAILURE = -1,
    PARSED_SUCCESS = 0
};

/**
 * TODO - Write API for below functions
 */
int parse_line(line_t *);
int skipable_line(char *);

#endif