#include "asmbl.h"
#include "misc.h"
#include "line.h"

#define SRC 1
#define DST 2

/**
 * Enumeration of all the addressing modes available
 * on this assembler.
 * Using multiples of 2 to allow easy additions
 */
enum ADDRESS_MODES
{
    ADDMODE_NONE = 0,
    ADDMODE_0 = 1,
    ADDMODE_1 = 2,
    ADDMODE_2 = 4,
    ADDMODE_3 = 8,
    ADDMODE_12 = ADDMODE_1 | ADDMODE_2,
    ADDMODE_13 = ADDMODE_1 | ADDMODE_3,
    ADDMODE_123 = ADDMODE_1 | ADDMODE_2 | ADDMODE_3,
    ADDMODE_ALL = ADDMODE_0 | ADDMODE_1 | ADDMODE_2 | ADDMODE_3
};


int const
opcodes[OPCODE_NUM][3] = 
{
/*  {OPCODE,  SOURCE , DESTINATION} */
    {MOV, ADDMODE_ALL, ADDMODE_123},
    {CMP, ADDMODE_ALL, ADDMODE_ALL},
    {ADD, ADDMODE_ALL, ADDMODE_123},
    {SUB, ADDMODE_ALL, ADDMODE_123},
    {NOT, ADDMODE_NONE, ADDMODE_123},
    {CLR, ADDMODE_NONE, ADDMODE_123},
    {LEA, ADDMODE_12, ADDMODE_123},
    {INC, ADDMODE_NONE, ADDMODE_123},
    {DEC, ADDMODE_NONE, ADDMODE_123},
    {JMP, ADDMODE_NONE, ADDMODE_13},
    {BNE, ADDMODE_NONE, ADDMODE_13},
    {RED, ADDMODE_NONE, ADDMODE_123},
    {PRN, ADDMODE_NONE, ADDMODE_ALL},
    {JSR, ADDMODE_NONE, ADDMODE_13},
    {RTS, ADDMODE_NONE, ADDMODE_NONE},
    {STOP, ADDMODE_NONE, ADDMODE_NONE}
};