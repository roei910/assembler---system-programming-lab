#include "baseConvertion.h"

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
}

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

void createBinaryCode(char **bin, int numOfArguments, ...){
    int numberOfArguments, i;
    va_list valist;
    va_start(valist, numOfArguments);
    
    for(i = 0 ; i < numOfArguments ; i++)
        printf("%d. %d\n", i, va_arg(valist, int));

    switch (numberOfArguments)
    {
        case 2:/*passing ARE, OPCODE*/
            
            break;
        case 6:/*passing ARE, FUNCT, REG ORIGIN, ORIGIN TYPE, REG DEST, ORIGIN DEST*/
            
            break;
        default:
            break;
    }
    va_end(valist);
}

void createBinaryLine(binLine *line, int numOfArguments, ...){
    int i;
    va_list valist;
    va_start(valist, numOfArguments);
    switch (numOfArguments)
    {
        case 2:/*passing indexes: ARE, OPCODE*/
            line->machineCode.code = pow(2, va_arg(valist, int)) + pow(2, va_arg(valist, int));
            printWord(*line);
            break;
        case 6:/*passing ARE(index), values: FUNCT, REG ORIGIN, ORIGIN TYPE, REG DEST, ORIGIN DEST*/
            line->machineCode.code = pow(2, va_arg(valist, int));
            line->machineCode.word.funct = va_arg(valist, int);
            line->machineCode.word.src = va_arg(valist, int);
            line->machineCode.word.addressingSrc = va_arg(valist, int);
            line->machineCode.word.dest = va_arg(valist, int);
            line->machineCode.word.addressingDest = va_arg(valist, int);
            printWord(*line);
            break;
        default:
            break;
    }
    va_end(valist);
}