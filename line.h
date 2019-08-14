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
 * Main parsing function.
 * Checks for label, macro, directive and instruction properties of current line.
 */
int parse_line(line_t *);

/**
 * Encode the current line to the symbol list and the DC/IC arrays
 */
int encode(enum PARSE parse, line_t *pLINE, symbol_node **list);
int entry_encode(char *entry, line_t *pLINE, symbol_node **list);
/**
 * Line that is commented or that is consists of whitespaces only
 */
int skipable_line(char *);
int skip_lines_sec_pass(line_t *pLINE);
int is_entry(line_t *pLINE, symbol_node *list);
int complete_encoding(symbol_node *list, line_t *pLINE, int oIC);

#endif