#ifndef MISC_H
#define MISC_H

#include "line.h"

#define SAFE_FREE(p) {free(p); (p) = NULL;}
#define ZERO_MEMORY(dst, len) memset((dst), 0, (len))
#define	ISASCII(c)	(((c) & ~0x7f) == 0)	/* If C is a 7 bit value.  */

/* Math functions */
int is_valid(int);

void LINE_FREE(line_t *);
int strcmp_hash(char *, char *);

#endif