#ifndef MISC_H
#define MISC_H

#include "line.h"

#define SAFE_FREE(p) {free(p); (p) = NULL;}
#define EMPTY_STRING ""

/* Printing an error whith the line in the assembly file */
#define ERROR_MSG(str) {fprintf(stderr, "Line %d - %s\n", line_num, str);} 

/* If c is a 7 bit value. */
#define	IS_ASCII(c)	(((c) & ~0x7f) == 0) 

#define ZERO_MEMORY(dst, len) memset((dst), 0, (len))

#define DECIMAL_BACE 10
#define INIT_PROTECTED_SIZE 10

extern unsigned long protected[INIT_PROTECTED_SIZE];

/* Trim whitespaces from end */
char *clear_str(char *str);
char *trim_white(char *str);

/* Math functions */
int is_valid(long num);
int is_num(char *num);

/* Free the whole line object */
void LINE_FREE(line_t *);

/* Hashing */
unsigned long hash(char *);
int strcmp_hash(char *, char *);
int is_reserved(char *);
int insert_protected(char *);

#endif