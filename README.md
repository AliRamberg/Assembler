# Assembler
Pseudo assembly language assembler

# Introduction
This repository is the final project of my C class course in the Open University of Israel
The project is merely the translator part of the whole Assembler software.

# The Machine
The CPU has 8 registers, each is of size of 14 bits.
registers: r0, r1, r2, r3, r4, r5, r6, r7.
It supports 16 types of opcodes:
    - mov
    - cmp
    - add
    - sub
    - not
    - clr
    - lea
    - inc
    - dec
    - jmp
    - bne
    - red
    - prn
    - jsr
    - rts
    - stop
There are 4096 memory cells of 14 bits each.
It supports only ascii characters and whole numbers, real numbers throws an error.

# Word architecture
+-----------+------------+------------+---------------+-------+
|      10-13|         6-9|         4-5|            2-3|    0-1|
|	    |            |            |               |       |
| Not Used  |   Opcode	 |   source   |  destination  | A.R.E |
|           |		 |            |               |       |
+-----------+------------+------------+---------------+-------+
# Note
The project is intended to work with ansi standard compiler
