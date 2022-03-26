#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include "constants.h"
#include "binaryLine.h"

/**
 * @file binaryLine.c
 * @author roei daniel
 * @brief the file will be able to handle print and write binary words
 * to a file or a binLine object 
 */

struct binaryLine{
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
};

void printWord(FILE *fp, BinaryLine *line){
    fprintf(fp, "A%x-B%x-C%x-D%x-E%x\n", line->machineCode.wordPrint.A, line->machineCode.wordPrint.B,
        line->machineCode.wordPrint.C, line->machineCode.wordPrint.D, line->machineCode.wordPrint.E);
}

void createBinaryLine(BinaryLine *line, int numOfArguments, ...){
    int ARE;
    va_list valist;
    va_start(valist, numOfArguments);
    switch (numOfArguments)
    {
        case 2:/*passing indexes: ARE, OPCODE*/
            ARE = va_arg(valist, int);
            line->machineCode.code = va_arg(valist, int);
            line->machineCode.word.A = (unsigned int)pow(2, ARE);
            break;
        case 4:/*passing ARE(index), values: FUNCT, DEST REG, DEST ADDRESSING*/
            line->machineCode.word.A = (unsigned int)pow(2, va_arg(valist, int));
            line->machineCode.word.funct = va_arg(valist, int);
            line->machineCode.word.src = 0;
            line->machineCode.word.addressingSrc = 0;
            line->machineCode.word.dest = va_arg(valist, int);
            line->machineCode.word.addressingDest = va_arg(valist, int);
            /*printWord(*line);*/
            break;
        case 6:/*passing ARE(index), values: FUNCT, SRC REG, SRC ADDRESSING, DEST REG, DEST ADDRESSING*/
            line->machineCode.word.A = (unsigned int)pow(2, va_arg(valist, int));
            line->machineCode.word.funct = va_arg(valist, int);
            line->machineCode.word.src = va_arg(valist, int);
            line->machineCode.word.addressingSrc = va_arg(valist, int);
            line->machineCode.word.dest = va_arg(valist, int);
            line->machineCode.word.addressingDest = va_arg(valist, int);
            /*printWord(*line);*/
            break;
        default:
            break;
    }
    va_end(valist);
}

BinaryLine *createBinaryLineArray(int size){
    return (BinaryLine *)calloc(size, sizeof(BinaryLine));
}

BinaryLine *getBinaryAtIndex(BinaryLine *binaryArray, int index){
    return binaryArray + index;
}