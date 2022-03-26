#include <stdio.h>
#include "decodeLine.h"
#include "binaryLine.h"
#include "constants.h"
#include "symbol.h"
#include "secondRun.h"

/**
 * @file secondRun.c
 * @author roei daniel
 * @brief start the second run on a file
 */

int startSecondRun(FILE *fp, char *extFileName, Symbol *symbolTable, BinaryLine *lines, int tableSize){
    char inputLine[MAX_LINE], tempSymbol[MAX_SYMBOL_LENGTH];
    char command[MAX_COMMAND_NAME_LENGTH], src[MAX_OPPERAND_LENGTH], dest[MAX_OPPERAND_LENGTH];
    int srcAddressing, destAddressing, numberOfOpperands;
    int error = 1, linesCounter = 0, commandLines = 0, fileLines = 0;
    FILE *extFp = NULL;
    rewind(fp);
    while(fgets(inputLine, MAX_LINE, fp) != NULL){
        fileLines++;
        if(isSymbolDecleration(inputLine))
            skipSymbol(inputLine);
        if(isEntryDecleration(inputLine)){
            if(extractSymbolFromEntryLine(inputLine, tempSymbol))
                addAttribute(linesCounter, symbolTable, tableSize, tempSymbol, ENTRY_ATTRIBUTE);
            else{
                fprintf(stdout, "[ERROR]: line:%d, couldn't extract symbol from entry line\n", fileLines);
                error = 0;
            }        
        }
        else if(!isEmptyLine(inputLine) && (!isDataDecleration(inputLine)) && (!isExternDecleration(inputLine)) && (!isCommentLine(inputLine))){
            commandLines = lineDecode(fileLines, inputLine, command, src, dest, &srcAddressing, &destAddressing, &numberOfOpperands);
            switch (numberOfOpperands)
            {
                case 1:
                    linesCounter+=2;
                    if(destAddressing == 0)
                        linesCounter++;
                    if(((destAddressing) == 1) || ((destAddressing) == 2)){/**/
                        if(!buildSymbolLines(fileLines, &extFp, extFileName, getBinaryAtIndex(lines, linesCounter), symbolTable, tableSize, dest, 100+linesCounter))
                            error = 0;
                        linesCounter+=2;
                    }
                    break;
                case 2:
                    linesCounter+=2;
                    if(srcAddressing == 0)
                        linesCounter++;
                    if((srcAddressing == 1) || (srcAddressing == 2)){
                        if(!buildSymbolLines(fileLines, &extFp, extFileName, getBinaryAtIndex(lines, linesCounter), symbolTable, tableSize, src, 100+linesCounter))
                            error = 0;  
                        linesCounter+=2;
                    }
                    if(destAddressing == 0)
                        linesCounter++;
                    if(((destAddressing) == 1) || ((destAddressing) == 2)){/**/
                        if(!buildSymbolLines(fileLines, &extFp, extFileName, getBinaryAtIndex(lines, linesCounter), symbolTable, tableSize, dest, 100+linesCounter))
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
    if(extFp != NULL)
        fclose(extFp);
    return error;
}

int addAttribute(int linesCounter, Symbol *table, int tableSize, char *symbolName, char *attr){
    int index = findSymbolInTable(table, tableSize, symbolName);
    int attrIndex;
    Symbol *symbolPtr = getSymbolAtIndex(table, index);
    if(index == -1){
        fprintf(stdout, "[ERROR]: line:%d, in entry line, symbol not found in symbol table\n", linesCounter);
        return 0;
    }
    attrIndex = getAttributesCount(symbolPtr);
    if(attrIndex >= MAX_ATTRIBUTES){
        fprintf(stdout, "[ERROR]: line:%d, max attributes for symbol \"%s\"\n", linesCounter, symbolName);
        return 0;
    }
    strcpy(getAttributesLine(symbolPtr, attrIndex), attr);
    increaseAttributeCount(symbolPtr);
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

int buildSymbolLines(int linesCounter, FILE **fp, char *extFileName, BinaryLine *lines, Symbol *symbolTable, int tableSize, char *opperand, int IC){
    int symbolIndex;
    char tempSymbol[MAX_SYMBOL_LENGTH];
    Symbol *symbolPtr;
    getSymbolFromOpperand(opperand, tempSymbol);
    symbolIndex = findSymbolInTable(symbolTable, tableSize, tempSymbol);
    symbolPtr = getSymbolAtIndex(symbolTable, symbolIndex);
    if(symbolIndex == -1){
        fprintf(stdout, "[ERROR]: line:%d, couldn't find symbol \"%s\" in the symbol table\n", linesCounter, tempSymbol);
        return 0;
    }
    if(!strcmp(getAttributesLine(symbolPtr, 0), EXTERNAL_ATTRIBUTE)){
        createBinaryLine(lines, 2, MACHINE_CODE_E, getBaseAddress(symbolPtr));
        createBinaryLine(getBinaryAtIndex(lines, 1), 2, MACHINE_CODE_E, getOffset(symbolPtr));
        if(*fp == NULL){
            if(!(*fp = fopen(extFileName, "w"))){
                fprintf(stdout, "[ERROR]: creating .ext file %s\n", extFileName);
                return 0;
            }  
        }
        printSymbolExternal(*fp, symbolPtr, IC);
    }
    else{
        createBinaryLine(lines, 2, MACHINE_CODE_R, getBaseAddress(symbolPtr));
        createBinaryLine(getBinaryAtIndex(lines, 1), 2, MACHINE_CODE_R, getOffset(symbolPtr));
    }
    return 1;             
}

void printSymbolExternal(FILE *fp, Symbol *extSymbol, int baseAddress){
    fprintf(fp, "%s BASE %d\n", getSymbolName(extSymbol), baseAddress);
    fprintf(fp, "%s OFFSET %d\n\n", getSymbolName(extSymbol), baseAddress+1);
}