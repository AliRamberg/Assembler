#ifndef MISC_H
#define MISC_H

#include "line.h"
#define SAFE_FREE(p) {free(p); (p) = NULL;}
#define ZERO_MEMORY(dst, len) memset((dst), 0, (len))

void LINE_FREE(line_t *);

#endif