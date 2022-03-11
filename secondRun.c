#include "secondRun.h"

int startSecondRun(FILE *fp, FILE *extFp, symbol *symbolTable, binLine *lines, int tableSize){
    char inputLine[MAX_LINE], tempSymbol[MAX_SYMBOL_LENGTH];
    char command[MAX_COMMAND_NAME_LENGTH], src[MAX_OPPERAND_LENGTH], dest[MAX_OPPERAND_LENGTH];
    int srcAddressing, destAddressing, numberOfOpperands;
    int error = 1, linesCounter = 0, commandLines = 0/*, symbolIndex*/;
    rewind(fp);
    while(fgets(inputLine, MAX_LINE, fp) != NULL){
        if(isSymbolDecleration(inputLine))
            skipSymbol(inputLine);
        if(isEntryDecleration(inputLine)){
            if(extractSymbolFromEntryLine(inputLine, tempSymbol))
                addAttribute(symbolTable, tableSize, tempSymbol, ENTRY_ATTRIBUTE);
            else{
                error = 0;
                fprintf(stderr, "[ERROR]: couldn't extract symbol from entry line\n");
            }        
        }
        else if((!isDataDecleration(inputLine)) && (!isExternDecleration(inputLine)) && (!strstr(inputLine, COMMENT_LINE_STRING))){
            commandLines = lineDecode(inputLine, command, src, dest, &srcAddressing, &destAddressing, &numberOfOpperands);
            switch (numberOfOpperands)
            {
                case 1:
                    linesCounter+=2;
                    if(destAddressing == 0)
                        linesCounter++;
                    if(((destAddressing) == 1) || ((destAddressing) == 2)){/**/
                        buildSymbolLines(extFp, lines+linesCounter, symbolTable, tableSize, dest, 100+linesCounter);
                        linesCounter+=2;
                    }
                    break;
                case 2:
                    linesCounter+=2;
                    if(srcAddressing == 0)
                        linesCounter++;
                    if((srcAddressing == 1) || (srcAddressing == 2)){
                        buildSymbolLines(extFp, lines+linesCounter, symbolTable, tableSize, src, 100+linesCounter);
                        linesCounter+=2;
                    }
                    if(destAddressing == 0)
                        linesCounter++;
                    if(((destAddressing) == 1) || ((destAddressing) == 2)){/**/
                        buildSymbolLines(extFp, lines+linesCounter, symbolTable, tableSize, dest, 100+linesCounter);
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

int isEntryDecleration(char *inputLine){
    if(checkDecleration(inputLine, ENTRY_DECLERATION))
        return 1;
    return 0;
}

int addAttribute(symbol *table, int tableSize, char *symbolName, char *attr){
    int index = findSymbolInTable(table, tableSize, symbolName);
    int attrIndex;
    if(index == -1){
        printf("\n");
        fprintf(stderr, "[ERROR]: in entry line, symbol not found in symbol table\n");
        return 0;/*symbol not found in table*/
    }
    attrIndex = (table+index)->attributeCount;
    if(attrIndex >= MAX_ATTRIBUTES)
        return -1;/*max attribues*/
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

int buildSymbolLines(FILE *fp, binLine *lines, symbol *symbolTable, int tableSize, char *opperand, int IC){
    int symbolIndex, error = 1;
    char tempSymbol[MAX_SYMBOL_LENGTH];
    getSymbolFromOpperand(opperand, tempSymbol);
    symbolIndex = findSymbolInTable(symbolTable, tableSize, tempSymbol);
    if(symbolIndex == -1){
        error = 0;
        fprintf(stderr, "[ERROR]: couldn't find symbol \"%s\" in the symbol table\n", tempSymbol);
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