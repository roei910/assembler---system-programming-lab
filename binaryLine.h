#include <stdio.h>
#include "constants.h"

#if !defined(BINARY_LINE_HEADER)
#define BINARY_LINE_HEADER
/*
typedef struct binaryLine BinaryLine;*/
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



#endif
