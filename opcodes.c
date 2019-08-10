#include "opcodes.h"

int num_operands(char *line);


unsigned
inst_size(char *line, unsigned ins)
{
    unsigned sz = 0;
    int ops = num_operands(line);
    int c_ops = 0;
    if(c_ops == ops)
    {

    }    
    else if (c_ops < ops)
    {
        ERROR_MSG("Not enough operands")
        fprintf(stderr, "Expected %d\n", c_ops);
        sz = -1;
    }
    else
    {
        ERROR_MSG("Too many operands")
        fprintf(stderr, "Expected %d\n", c_ops);
        sz = -1;
    }
    
    return sz;
}


int
num_operands(char *line)
{
    int ops = 0;
    char *c = line;
    while(*c)
    {
        if(*c++ == ',')
            ops++;
    }
    return ops;
}