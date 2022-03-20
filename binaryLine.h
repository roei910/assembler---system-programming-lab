#if !defined(BINARY_LINE_HEADER)
#define BINARY_LINE_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include "constants.h"

/**
 * @brief binary Line struct contains the binary code for a line 
 */
typedef struct binaryLine BinaryLine;

/**
 * @brief print a binary word to a file fp
 * 
 * @param fp file pointer
 * @param line the binary line to be written
 */
void printWord(FILE *, BinaryLine *);

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

/**
 * @brief Create a Binary Line Array
 * 
 * @param size size of array
 * @return BinaryLine* 
 */
BinaryLine *createBinaryLineArray(int);

/**
 * @brief Get the Binary At Index from binary array
 * 
 * @param binaryArray pointer to binary array
 * @param index int index
 * @return BinaryLine* 
 */
BinaryLine *getBinaryAtIndex(BinaryLine *, int);

#endif
