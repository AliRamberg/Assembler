#pragma once
struct line_s
{
    char *line;
    char *first;
    char *second;
    char *third;
    char *fourth;
    char *fifth;
};
typedef struct line_s line_t;

int parse_line(line_t *);