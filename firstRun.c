#include "firstRun.h"

/**
 * @brief start a first run of a file fp, returns FALSE if the run was successfull
 * 
 * @param fp 
 * @param symbolTable 
 * @param lines 
 * @param ICF 
 * @param DCF 
 * @param tableSize 
 * @return int 
 */
int startFirstRun(FILE *fp, symbol *symbolTable, BinaryLine *lines, int *ICF, int *DCF, int *tableSize){
    int IC = 100, DC = 0, symbolDecleration, symbolCount = 0, addedLines, error = 1, fileLines = 0;
    int srcAddressing, destAddressing, numberOfOpperands;
    char command[MAX_COMMAND_NAME_LENGTH], src[MAX_OPPERAND_LENGTH], dest[MAX_OPPERAND_LENGTH];
    char inputLine[MAX_LINE], tempSymbol[MAX_SYMBOL_LENGTH];
    while(fgets(inputLine, MAX_LINE, fp) != NULL){
        fileLines++;
        symbolDecleration = 0;
        if(isSymbolDecleration(inputLine)){
            symbolDecleration = 1;
        }
        if(isDataDecleration(inputLine)){
            if(symbolDecleration){
                extractSymbol(inputLine, tempSymbol);
                if(!checkValidSymbol(tempSymbol)){
                    fprintf(stderr, "[ERROR]: line:%d, invalid symbol \"%s\"\n", fileLines, tempSymbol);
                    error = 0;
                }
                if(findSymbolInTable(symbolTable, symbolCount, tempSymbol) == -1)
                    createSymbol(symbolTable, symbolCount++, tempSymbol, DATA_ATTRIBUTE, 16 * (IC / 16), IC - (16 * (IC / 16)));
                else{
                    fprintf(stderr, "[ERROR]: line:%d, symbol \"%s\" already exists\n", fileLines, tempSymbol);
                    error = 0;
                }
            }
            /*7 - add data as bin lines, add DC count according to lines created*/
            addedLines = extractDataFromLine(inputLine, lines+IC-100, fileLines);
            if(addedLines == 0)
                error = 0;
            DC += addedLines;
            IC += addedLines;
        }
        else if(isExternDecleration(inputLine)){
            if(symbolDecleration)
                skipSymbol(inputLine);
            if(sscanf(inputLine+strlen(EXTERN_DECLERATION), " %s", tempSymbol) == -1){
                fprintf(stderr, "[ERROR]: line:%d, missing symbol after .extern\n", fileLines);
                error = 0;
            }
            else{
                if(!checkValidSymbol(tempSymbol)){/*check symbol is valid*/
                    fprintf(stderr, "[ERROR]: line:%d, invalid symbol \"%s\"\n", fileLines, tempSymbol);
                    error = 0;
                }
                if(findSymbolInTable(symbolTable, symbolCount, tempSymbol) == -1)
                    createSymbol(symbolTable, symbolCount++, tempSymbol, EXTERNAL_ATTRIBUTE, 0, 0);
                else{
                    fprintf(stderr, "[ERROR]: line:%d, symbol \"%s\" already exists\n", fileLines, tempSymbol);
                    error = 0;
                }
            }
        }
        else if(isEntryDecleration(inputLine)){
            if(symbolDecleration)
                skipSymbol(inputLine);
            if(sscanf(inputLine+strlen(ENTRY_DECLERATION), " %s", tempSymbol) == -1){
                fprintf(stderr, "[ERROR]: line:%d, missing symbol after .entry\n", fileLines);
                error = 0;
            }
        }
        else if((!isEmptyLine(inputLine)) && (!isCommentLine(inputLine))){/*11 - normal code line*/
            if(symbolDecleration){
                extractSymbol(inputLine, tempSymbol);
                if(!checkValidSymbol(tempSymbol)){
                    fprintf(stderr, "[ERROR]: line:%d, invalid symbol \"%s\"\n", fileLines, tempSymbol);
                    error = 0;
                }
                if(findSymbolInTable(symbolTable, symbolCount, tempSymbol) == -1)
                    createSymbol(symbolTable, symbolCount++, tempSymbol, CODE_ATTRIBUTE, 16 * (IC / 16), IC - 16 * (IC / 16));
                else{
                    fprintf(stderr, "[ERROR]: line:%d, symbol \"%s\" already exists\n", fileLines, tempSymbol);
                    error = 0;
                }
                skipSymbol(inputLine);
            }
            addedLines = lineDecode(fileLines, inputLine, command, src, dest, &srcAddressing, &destAddressing, &numberOfOpperands);
            if(!addedLines){
                error = 0;
            }
            else if(!buildCodeLines(fileLines, numberOfOpperands, lines+IC-100, symbolTable, symbolCount, command, src, srcAddressing,dest, destAddressing)){
                error = 0;
            }
            IC += addedLines;            
        }
    }
    *ICF = IC;
    *DCF = DC;
    *tableSize = symbolCount;
    return error;
}

/**
 * @brief returns true if an inputline contains .string
 * 
 * @param inputLine 
 * @return int 
 */
int isStringLine(char *inputLine){
    if(strstr(inputLine, STRING_DECLERATION))
        return 1;
    return 0;
}

/**
 * @brief Creates a Symbol in the symbol table
 * 
 * @param table 
 * @param index 
 * @param symbolName 
 * @param attr 
 * @param base 
 * @param offset 
 */
void createSymbol(symbol *table, int index, char *symbolName, char *attr ,int base, int offset){
    strcpy((table + (index))->symbol, symbolName);
    (table+index)->baseAddress = base;
    (table+index)->offset = offset;
    strcpy(((table+index)->attributes)[0], attr);
    ((table+index)->attributeCount) = 1;
}

/**
 * @brief creates .string and .data binary lines, returns the amount of lines created
 * 
 * @param inputLine 
 * @param lines 
 * @return int 
 */
int extractDataFromLine(char *inputLine, BinaryLine *lines, int line){
    int tempNumber, countLines = 0, flag = 0;
    char tempC;
    char *ptr, tempLine[MAX_LINE];
    if(isStringLine(inputLine)){/*.string*/
        ptr =  strstr(inputLine, QUOTATION_SYMBOL);
        if(ptr == NULL){
            fprintf(stderr, "[ERROR]: line:%d, string is incorrect / missing\n", line);
            return 0;
        }
        strcpy(tempLine, ptr + 1);
        ptr = tempLine;
        while((*ptr) != QUOTATION_SYMBOL_CHAR){
            tempC = *(ptr++);
            if(tempC == '"')
                tempC = 0;
            createBinaryLine(lines+countLines, 2, MACHINE_CODE_A, tempC);
            countLines++;
            if(*ptr == '\n'){
                fprintf(stderr, "[ERROR]: line:%d, string is incorrect\n", line);
                return 0;
            }
        }
        createBinaryLine(lines+countLines, 2, MACHINE_CODE_A, 0);
            countLines++;
    }
    else{/*.data*/
        strcpy(tempLine, strstr(inputLine, DATA_DECLERATION) + 5);
        if(isEmptyLine(tempLine)){
            fprintf(stderr, "[ERROR]: line:%d, data missing\n", line);
            return 0;
        }
        ptr = strtok(tempLine, COMMA_SYMBOL);
        flag = sscanf(ptr, " %c", &tempC);
        if(flag == -1){
            fprintf(stderr, "[ERROR]: line:%d, illegal comma in data line\n", line);
            return 0;
        }
        while(ptr){
            flag = sscanf(ptr, "%d", &tempNumber);
            if(flag == 0){
                fprintf(stderr, "[ERROR]: line:%d, incorrect data\n", line);
                return 0;
            }
            else if(flag == -1){
                fprintf(stderr, "[ERROR]: line:%d, illegal comma in data line\n", line);
                return 0;
            }
            ptr = strtok(NULL, COMMA_SYMBOL);
            createBinaryLine(lines+countLines, 2, MACHINE_CODE_A, tempNumber);
            countLines++;
        }
    }
    return countLines;
}

/**
 * @brief build machine code lines to binLine array according to command type
 * 
 * @param linesCounter 
 * @param numberOfOpperands 
 * @param lines 
 * @param symbolTable 
 * @param symbolCount 
 * @param command 
 * @param src 
 * @param srcAddressing 
 * @param dest 
 * @param destAddressing 
 * @return int 
 */
int buildCodeLines(int linesCounter, int numberOfOpperands, BinaryLine *lines, symbol *symbolTable, int symbolCount,
    char *command, char *src, int srcAddressing, char *dest, int destAddressing){
    switch (numberOfOpperands)
    {
        case 0:
            return buildMachineCodeLines(linesCounter ,lines, symbolCount, symbolTable, command, 0);
        case 1:
            return buildMachineCodeLines(linesCounter ,lines, symbolCount, symbolTable, command, 2, dest, destAddressing);
        case 2:
            return buildMachineCodeLines(linesCounter ,lines, symbolCount, symbolTable, command, 4, src, srcAddressing, dest, destAddressing);
    }
    return 0;
}

/**
 * @brief extracts a symbol from inputline and copies the symbol to symbol parameter
 * 
 * @param inputLine 
 * @param symbol 
 */
void extractSymbol(char *inputLine, char *symbol){
    char *ptr, tempLine[MAX_LINE];
    strcpy(tempLine, inputLine);
    ptr = strtok(tempLine, LABEL_SYMBOL);
    strcpy(symbol, ptr);
}
