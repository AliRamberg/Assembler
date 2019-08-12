/* asmbl.h -- primary header for main */
#ifndef ASMBL_H
#define ASMBL_H

#include <stdlib.h>
#include <stdio.h>
#include "line.h"

#define _12BIT_MAX 0xFFF  /* Biggest number for integer in word_t */
#define _12BIT_MIN -0xFFF  /* Smallest number for integer in word_t */
/********************************************************************/
/* Error Codes */
#define NO_FILE 127
#define EXCEEDS_MAX_LENGTH 2
#define PARSING_FAILURE 3

#define MEM_SIZE 4096      /* Number of cells of memory, each 14 bits */
#define OPCODE_NUM 16
#define REGISTER_NUM 8

#define INITIAL_IC_ARR 100
#define INITIAL_DC_ARR 100

/**
 * Opcodes Instructions
 */
#define MOV 0
#define CMP 1
#define ADD 2
#define SUB 3
#define NOT 4
#define CLR 5
#define LEA 6
#define INC 7
#define DEC 8
#define JMP 9
#define BNE 10
#define RED 11
#define PRN 12
#define JSR 13
#define RTS 14
#define STOP 15
/**********************/


#define NEGATIVE '-'
#define POSITIVE '+'
enum BOOL {FALSE, TRUE};



/**
 * word datatype
 * each is of size of 14 bits.
 */
struct WORD_T
{
    signed int reg:14;
};
typedef struct WORD_T WORD_T;


int line_num; /* current line number */
int IC;  /* Instruction Counter */
int DC; /* Data Conter */

/* Instructions and Data binary words */
WORD_T instruction_arr[INITIAL_IC_ARR];
WORD_T data_arr[INITIAL_DC_ARR];

#endif