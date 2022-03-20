/**
 * @file secondRun.c
 * @author roei daniel
 * @brief start the second run on a file
 */
#include "secondRun.h"

/**
 * @brief the function will start second run, will return FALSE if found errors
 * 
 * @param fp 
 * @param extFp 
 * @param symbolTable 
 * @param lines 
 * @param tableSize 
 * @return int 
 */
int startSecondRun(FILE *fp, char *extFileName, symbol *symbolTable, BinaryLine *lines, int tableSize){
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
                        if(!buildSymbolLines(fileLines, &extFp, extFileName, lines+linesCounter, symbolTable, tableSize, dest, 100+linesCounter))
                            error = 0;
                        linesCounter+=2;
                    }
                    break;
                case 2:
                    linesCounter+=2;
                    if(srcAddressing == 0)
                        linesCounter++;
                    if((srcAddressing == 1) || (srcAddressing == 2)){
                        if(!buildSymbolLines(fileLines, &extFp, extFileName, lines+linesCounter, symbolTable, tableSize, src, 100+linesCounter))
                            error = 0;  
                        linesCounter+=2;
                    }
                    if(destAddressing == 0)
                        linesCounter++;
                    if(((destAddressing) == 1) || ((destAddressing) == 2)){/**/
                        if(!buildSymbolLines(fileLines, &extFp, extFileName, lines+linesCounter, symbolTable, tableSize, dest, 100+linesCounter))
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

/**
 * @brief adds an attribute to the symbol from symbol table array
 * 
 * @param linesCounter 
 * @param table 
 * @param tableSize 
 * @param symbolName 
 * @param attr 
 * @return int 
 */
int addAttribute(int linesCounter, symbol *table, int tableSize, char *symbolName, char *attr){
    int index = findSymbolInTable(table, tableSize, symbolName);
    int attrIndex;
    if(index == -1){
        fprintf(stdout, "[ERROR]: line:%d, in entry line, symbol not found in symbol table\n", linesCounter);
        return 0;
    }
    attrIndex = (table+index)->attributeCount;
    if(attrIndex >= MAX_ATTRIBUTES){
        fprintf(stdout, "[ERROR]: line:%d, max attributes for symbol \"%s\"\n", linesCounter, symbolName);
        return 0;
    }
    strcpy(((table+index)->attributes)[attrIndex], attr);
    ((table+index)->attributeCount)++;
    return 1;
}

/**
 * @brief extracts the symbol from the entry line to tempsymbol parameter
 * 
 * @param inputLine 
 * @param tempSymbol 
 * @return int 
 */
int extractSymbolFromEntryLine(char *inputLine, char *tempSymbol){
    return sscanf(strstr(inputLine, ENTRY_DECLERATION) + strlen(ENTRY_DECLERATION), "%s", tempSymbol);
}

/**
 * @brief Get the Symbol From an Opperand to the tempsymbol
 * 
 * @param opperand 
 * @param tempSymbol 
 */
void getSymbolFromOpperand(char *opperand, char *tempSymbol){
    char *ptr;
    if((ptr = strstr(opperand, LEFT_SQUARE_BRACKET))){
        strncpy(tempSymbol, opperand, ptr - opperand);
        tempSymbol[ptr-opperand] = '\0';
    }
    else
        sscanf(opperand, "%s", tempSymbol);
}

/**
 * @brief build the symbol lines to the binline array
 * 
 * @param linesCounter 
 * @param fp 
 * @param lines 
 * @param symbolTable 
 * @param tableSize 
 * @param opperand 
 * @param IC 
 * @return int 
 */
int buildSymbolLines(int linesCounter, FILE **fp, char *extFileName, BinaryLine *lines, symbol *symbolTable, int tableSize, char *opperand, int IC){
    int symbolIndex, error = 1;
    char tempSymbol[MAX_SYMBOL_LENGTH];
    getSymbolFromOpperand(opperand, tempSymbol);
    symbolIndex = findSymbolInTable(symbolTable, tableSize, tempSymbol);
    if(symbolIndex == -1){
        fprintf(stdout, "[ERROR]: line:%d, couldn't find symbol \"%s\" in the symbol table\n", linesCounter, tempSymbol);
        error = 0;
    }
    if(!strcmp(((symbolTable+symbolIndex)->attributes[0]), EXTERNAL_ATTRIBUTE)){
        createBinaryLine(lines, 2, MACHINE_CODE_E, (symbolTable+symbolIndex)->baseAddress);
        createBinaryLine(lines+1, 2, MACHINE_CODE_E, (symbolTable+symbolIndex)->offset);
        if(*fp == NULL){
            if(!(*fp = fopen(extFileName, "w"))){
                fprintf(stdout, "[ERROR]: creating .ext file %s\n", extFileName);
                error = 0;
            }  
        }
        printSymbolExternal(*fp, symbolTable+symbolIndex, IC);
    }
    else{
        createBinaryLine(lines, 2, MACHINE_CODE_R, (symbolTable+symbolIndex)->baseAddress);
        createBinaryLine(lines+1, 2, MACHINE_CODE_R, (symbolTable+symbolIndex)->offset);
    }
    return error;             
}

/**
 * @brief prints the use of a symbol to the ext file
 * 
 * @param fp 
 * @param extSymbol 
 * @param baseAddress 
 */
void printSymbolExternal(FILE *fp, symbol *extSymbol, int baseAddress){
    fprintf(fp, "%s BASE %d\n", extSymbol->symbol, baseAddress);
    fprintf(fp, "%s OFFSET %d\n\n", extSymbol->symbol, baseAddress+1);
}