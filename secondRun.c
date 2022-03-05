#include "secondRun.h"

int startSecondRun(FILE *fp, symbol *symbolTable, binLine *lines, int tableSize){
    char inputLine[MAX_LINE], tempSymbol[MAX_SYMBOL_LENGTH];
    int error = 1, linesCounter = 0;
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
            linesCounter++;
        }
        else if((!isDataDecleration(inputLine)) && (!isExternDecleration(inputLine)) && (!strstr(inputLine, COMMENT_LINE_STRING))){
            /*printf("there is a normal line, need to complete the binary line\n");*/
            
        }
    }
    return error;
}

int writeSymbolLine(binLine *lines, int baseAddress, int offset){
    createBinaryLine(lines, 2, MACHINE_CODE_R, baseAddress);
    createBinaryLine(lines+1, 2, MACHINE_CODE_R, offset);
    return 1;
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
    /*printf("index = %d, attrIndex = %d\n", index, attrIndex);*/
    if(attrIndex >= MAX_ATTRIBUTES)
        return -1;/*max attribues*/
    strcpy(((table+index)->attributes)[attrIndex], attr);
    ((table+index)->attributeCount)++;
    return 1;
}

int extractSymbolFromEntryLine(char *inputLine, char *tempSymbol){
    return sscanf(strstr(inputLine, ENTRY_DECLERATION) + strlen(ENTRY_DECLERATION), "%s", tempSymbol);
}

