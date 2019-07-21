#include <stdio.h>
#include <stdlib.h>
#include "line.h"
#include "misc.h"
/* Free the line object and its dynamically allocated elements */
void LINE_FREE(line_t *oLINE)
{
    if(oLINE->label)
        SAFE_FREE(oLINE->label);
    SAFE_FREE(oLINE);
}