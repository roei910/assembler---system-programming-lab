#include "baseConvertion.h"

/*
int main(){
    int number = 55;
    int bits = 16;
    char *tempBinaryNumber;
    binLine *line;
    numberToBinary(number, &tempBinaryNumber, bits);
    printBinary(tempBinaryNumber, bits);
    printf("number = %d\n", binaryToNumber(tempBinaryNumber, bits));
    createBinaryCode(&tempBinaryNumber, 2, 18,2);
    createBinaryLine(line, 2, 18, 13);
    createBinaryLine(line, 6, 18, 10, 3, 3, 0, 1);
    return 0;
}*/

void printBinary(char *bin, int bits){
    int i;
    for(i = 0 ; i < bits ; i++){
        printf("%d", bin[i]);
    }
    printf("\n");
}

void printWord(binLine line){
    printf("A%x-B%x-C%x-D%x-E%x\n", line.machineCode.wordPrint.A, line.machineCode.wordPrint.B,
        line.machineCode.wordPrint.C, line.machineCode.wordPrint.D, line.machineCode.wordPrint.E);
}

int binaryToNumber(char *bin, int bits){
    int number=0, i, y;
    for(i = bits-1, y = 0 ; i >= 0 ; i--, y++){
        number+=pow(2, y)*bin[i];
    }
    return number;
}

void numberToBinary(int number, char **bin, int bits){
    int i;
    char *cp = (char *)calloc(bits, sizeof(char));
    *bin = cp;
    for(i = bits-1 ; i >= 0 ; i--, number>>=1)
        cp[i] = (number & 1) ? 1 : 0;
}

void createBinaryLine(binLine *line, int numOfArguments, ...){
    va_list valist;
    va_start(valist, numOfArguments);
    switch (numOfArguments)
    {
        case 0:/*empty line for symbol*/
            line->machineCode.code = 0;
        case 2:/*passing indexes: ARE, OPCODE*/
            line->machineCode.code = pow(2, va_arg(valist, int)) + va_arg(valist, int);
            /*printWord(*line);*/
            break;
        case 4:/*passing ARE(index), values: FUNCT, DEST REG, DEST ADDRESSING*/
            line->machineCode.code = pow(2, va_arg(valist, int));
            line->machineCode.word.funct = va_arg(valist, int);
            line->machineCode.word.src = 0;
            line->machineCode.word.addressingSrc = 0;
            line->machineCode.word.dest = va_arg(valist, int);
            line->machineCode.word.addressingDest = va_arg(valist, int);
            /*printWord(*line);*/
            break;
        case 6:/*passing ARE(index), values: FUNCT, ORIGIN REG, ORIGIN ADDRESSING, DEST REG, DEST ADDRESSING*/
            line->machineCode.code = pow(2, va_arg(valist, int));
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