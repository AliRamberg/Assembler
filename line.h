#ifndef LINE_H
#define LINE_H

#define LABEL_LEN 10
#define LINE_LEN 80       /* Line length */
typedef struct line_s
{
    char *line;
    char *label;
    char *second;
    char *third;
    char *fourth;
    char *fifth;
} line_t;


/**
 * TODO - Write API for below functions
 */
int parse_line(line_t *);
char *is_label(char *);
int skipable_line(char *);
int is_macro(line_t *);

#endif