#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include "constants.h"

#if !defined(BINARY_LINE_HEADER)
#define BINARY_LINE_HEADER

/**
 * @brief binary Line struct contains the binary code for a line 
 */
typedef struct binaryLine{
    union MachineCode{
        unsigned int code:SIZE_OF_WORD;
        struct 
        {
            unsigned int addressingDest:2;
            unsigned int dest:4;
            unsigned int addressingSrc:2;
            unsigned int src:4;
            unsigned int funct:4;
            unsigned int A:4;
        }word;

        struct 
        {
            unsigned int E:4;
            unsigned int D:4;
            unsigned int C:4;
            unsigned int B:4;
            unsigned int A:4;
        }wordPrint;
    } machineCode;
} BinaryLine;

/**
 * @brief print a binary word to a file fp
 * 
 * @param fp file pointer
 * @param line the binary line to be written
 */
void printWord(FILE *, BinaryLine);

/**
 * @brief Create Binary Line according to the desired type of command
 * 2 arguments - pass an index value of ARE and OPCODE
 * 4 arguments - pass a whole line only with destination opperands
 * 6 arguments - pass a whole line with destination and source opperands
 * 
 * @param line the binary line array to receive the binary lines
 * @param numOfArguments amount of arguments to the function
 * @param ... according to the desired binary line will receive arguments
 */
void createBinaryLine(BinaryLine *, int, ...);

#endif
