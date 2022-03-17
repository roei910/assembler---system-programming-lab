/**
 * @file baseConvertion.c
 * @author roei daniel
 * @brief the file will be able to handle print and write binary words
 * to a file or a binLine object 
 */

#include "baseConvertion.h"
/**
 * @brief print a binary word to a file fp
 * 
 * @param fp file pointer
 * @param line the binary line to be written
 */
void printWord(FILE *fp, BinaryLine line){
    fprintf(fp, "A%x-B%x-C%x-D%x-E%x\n", line.machineCode.wordPrint.A, line.machineCode.wordPrint.B,
        line.machineCode.wordPrint.C, line.machineCode.wordPrint.D, line.machineCode.wordPrint.E);
}

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