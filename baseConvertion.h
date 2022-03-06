#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#if !defined(CONSTANTS_HEADER)
#define CONSTANTS_HEADER
#include "constants.h"
#endif

#define NUMBER_OF_BITS 16
#define MACHINE_COMMAND_BITS 20
void printBinary(char *, int);
int binaryToNumber(char *, int );
void numberToBinary(int, char **, int);
void printWord(FILE *, binLine);
void createBinaryLine(binLine *, int, ...);