#pragma once
#define LABEL_LEN 10
#define LINE_LEN 80       /* Line length */
struct line_s
{
    char *line;
    char *label;
    char *second;
    char *third;
    char *fourth;
    char *fifth;
};
typedef struct line_s line_t;

int parse_line(line_t *);
char *is_label(char *);
int is_macro(line_t *);