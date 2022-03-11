#include "secondRun.h"

int startSecondRun(FILE *fp, FILE *extFp, symbol *symbolTable, binLine *lines, int tableSize){
    char inputLine[MAX_LINE], tempSymbol[MAX_SYMBOL_LENGTH];
    char command[MAX_COMMAND_NAME_LENGTH], src[MAX_OPPERAND_LENGTH], dest[MAX_OPPERAND_LENGTH];
    int srcAddressing, destAddressing, numberOfOpperands;
    int error = 1, linesCounter = 0, commandLines = 0;
    rewind(fp);
    while(fgets(inputLine, MAX_LINE, fp) != NULL){
        if(isSymbolDecleration(inputLine))
            skipSymbol(inputLine);
        if(isEntryDecleration(inputLine)){
            if(extractSymbolFromEntryLine(inputLine, tempSymbol))
                addAttribute(linesCounter, symbolTable, tableSize, tempSymbol, ENTRY_ATTRIBUTE);
            else{
                fprintf(stderr, "[ERROR]: line:%d, couldn't extract symbol from entry line\n", linesCounter);
                error = 0;
            }        
        }
        else if(!isEmptyLine(inputLine) && (!isDataDecleration(inputLine)) && (!isExternDecleration(inputLine)) && isCommentLine(inputLine)){
            commandLines = lineDecode(linesCounter, inputLine, command, src, dest, &srcAddressing, &destAddressing, &numberOfOpperands);
            switch (numberOfOpperands)
            {
                case 1:
                    linesCounter+=2;
                    if(destAddressing == 0)
                        linesCounter++;
                    if(((destAddressing) == 1) || ((destAddressing) == 2)){/**/
                        if(!buildSymbolLines(linesCounter, extFp, lines+linesCounter, symbolTable, tableSize, dest, 100+linesCounter))
                            error = 0;
                        linesCounter+=2;
                    }
                    break;
                case 2:
                    linesCounter+=2;
                    if(srcAddressing == 0)
                        linesCounter++;
                    if((srcAddressing == 1) || (srcAddressing == 2)){
                        if(!buildSymbolLines(linesCounter, extFp, lines+linesCounter, symbolTable, tableSize, src, 100+linesCounter))
                            error = 0;  
                        linesCounter+=2;
                    }
                    if(destAddressing == 0)
                        linesCounter++;
                    if(((destAddressing) == 1) || ((destAddressing) == 2)){/**/
                        if(!buildSymbolLines(linesCounter, extFp, lines+linesCounter, symbolTable, tableSize, dest, 100+linesCounter))
                            error = 0;
                        linesCounter+=2;
                    }
                    break;
                default:
                    linesCounter += commandLines;
                    break;
            }
        }
    }
    return error;
}

int addAttribute(int linesCounter, symbol *table, int tableSize, char *symbolName, char *attr){
    int index = findSymbolInTable(table, tableSize, symbolName);
    int attrIndex;
    if(index == -1){
        printf("\n");
        fprintf(stderr, "[ERROR]: line:%d, in entry line, symbol not found in symbol table\n", linesCounter);
        return 0;
    }
    attrIndex = (table+index)->attributeCount;
    if(attrIndex >= MAX_ATTRIBUTES){
        fprintf(stderr, "[ERROR]: line:%d, max attributes for symbol \"%s\"\n", linesCounter, symbolName);
        return 0;
    }
    strcpy(((table+index)->attributes)[attrIndex], attr);
    ((table+index)->attributeCount)++;
    return 1;
}

int extractSymbolFromEntryLine(char *inputLine, char *tempSymbol){
    return sscanf(strstr(inputLine, ENTRY_DECLERATION) + strlen(ENTRY_DECLERATION), "%s", tempSymbol);
}

void getSymbolFromOpperand(char *opperand, char *tempSymbol){
    char *ptr;
    if((ptr = strstr(opperand, LEFT_SQUARE_BRACKET))){
        strncpy(tempSymbol, opperand, ptr - opperand);
        tempSymbol[ptr-opperand] = '\0';
    }
    else
        sscanf(opperand, "%s", tempSymbol);
}

int buildSymbolLines(int linesCounter, FILE *fp, binLine *lines, symbol *symbolTable, int tableSize, char *opperand, int IC){
    int symbolIndex, error = 1;
    char tempSymbol[MAX_SYMBOL_LENGTH];
    getSymbolFromOpperand(opperand, tempSymbol);
    symbolIndex = findSymbolInTable(symbolTable, tableSize, tempSymbol);
    if(symbolIndex == -1){
        fprintf(stderr, "[ERROR]: line:%d, couldn't find symbol \"%s\" in the symbol table\n", linesCounter, tempSymbol);
        error = 0;
    }
    if(!strcmp(((symbolTable+symbolIndex)->attributes[0]), EXTERNAL_ATTRIBUTE)){
        createBinaryLine(lines, 2, MACHINE_CODE_E, (symbolTable+symbolIndex)->baseAddress);
        createBinaryLine(lines+1, 2, MACHINE_CODE_E, (symbolTable+symbolIndex)->offset);
        printSymbolExternal(fp, symbolTable+symbolIndex, IC);
    }
    else{
        createBinaryLine(lines, 2, MACHINE_CODE_R, (symbolTable+symbolIndex)->baseAddress);
        createBinaryLine(lines+1, 2, MACHINE_CODE_R, (symbolTable+symbolIndex)->offset);
    }
    return error;             
}

void printSymbolExternal(FILE *fp, symbol *extSymbol, int baseAddress){
    fprintf(fp, "%s BASE %d\n", extSymbol->symbol, baseAddress);
    fprintf(fp, "%s OFFSET %d\n\n", extSymbol->symbol, baseAddress+1);
}