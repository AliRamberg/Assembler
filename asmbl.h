/* asmbl.h -- primary header for main */
#include <stdlib.h>
#include <stdio.h>
#define ASMBL_H
#define _13BIT_MAX 0x1FFF  /* Biggest number for integer in word_t */
#define _13BIT_MIN 0xE000  /* Smallest number for integer in word_t */
#define MEM_SIZE 4096      /* Number of cells of memory, each 14 bits */
#define OPCODE_NUM 16

#define safe_free(p) {free(p); (p) = NULL;}

#define mov 1
#define cmp 2
#define add 3
#define sub 4

#define IMMIDIATE 0

enum BOOL {FALSE, TRUE};

enum ARE_E
{
    A = 0,
    R = 1,
    E = 2
};


/**
 * word datatype
 * each is of size of 14 bits.
 */
struct
{
    unsigned int are:2;     /* A.R.E addressing method */
    unsigned int dst:2;     /* destination operand */
    unsigned int src:2;     /* source operand */
    unsigned int opcd:4;    /* opcode */
    unsigned int unusd:4;   /* unused: initialized to 0 */
} word_t;
typedef struct word_t WORD_T;

struct
{
    char *name;
    unsigned int reg:14;
} _register_t;
typedef struct _register_t REGISTER;


union 
{
    line_t *guidance;
    line_t *command;
    line_t *macro;
} line_u_;
