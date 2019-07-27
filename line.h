#ifndef LINE_H
#define LINE_H

#define LINE_LEN 80       /* Line maximum length */
#define LABEL_LEN 31      /* Label maximum length */
#define MACRO_LEN 20      /* macro maximum length - length of ".define" */

#define HASH_DEFINE hash(".define")

typedef struct line_s
{
    char *line;
    char *label;
    char *macro;
} line_t;

enum DIRECTIVE 
{
    DIRECTIVE_DATA, 
    DIRECTIVE_STRING, 
    DIRECTIVE_ENTRY, 
    DIRECTIVE_EXTERN
};

/**
 * TODO - Write API for below functions
 */
int parse_line(line_t *);
int skipable_line(char *);

#endif