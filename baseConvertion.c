#include "baseConvertion.h"

void printWord(FILE *fp, binLine line){
    fprintf(fp, "A%x-B%x-C%x-D%x-E%x\n", line.machineCode.wordPrint.A, line.machineCode.wordPrint.B,
        line.machineCode.wordPrint.C, line.machineCode.wordPrint.D, line.machineCode.wordPrint.E);
}

void createBinaryLine(binLine *line, int numOfArguments, ...){
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

/*
void printBinary(char *bin, int bits){
    int i;
    for(i = 0 ; i < bits ; i++){
        printf("%d", bin[i]);
    }
    printf("\n");
}*/

/*
int binaryToNumber(char *bin, int bits){
    int number=0, i, y;
    for(i = bits-1, y = 0 ; i >= 0 ; i--, y++){
        number+=pow(2, y)*bin[i];
    }
    return number;
}*/

/*
void numberToBinary(int number, char **bin, int bits){
    int i;
    char *cp = (char *)calloc(bits, sizeof(char));
    *bin = cp;
    for(i = bits-1 ; i >= 0 ; i--, number>>=1)
        cp[i] = (number & 1) ? 1 : 0;
}*/