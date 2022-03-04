#include "secondRun.h"

int startSecondRun(FILE *fp){
    char inputLine[MAX_LINE];
    while(fgets(inputLine, MAX_LINE, fp) != NULL){
        
    }
}

int writeSymbolLine(binLine *lines, int baseAddress, int offset){
    createBinaryLine(lines, 2, MACHINE_CODE_R, baseAddress);
    createBinaryLine(lines+1, 2, MACHINE_CODE_R, offset);
    return 1;
}