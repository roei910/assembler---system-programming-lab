#include "firstRun.h"

/*
add symbol:
int i;
    symbol *table = (symbol *) malloc(8 * sizeof(symbol));
    addSymbol(table, 0, "W", "external", 0, 0);
    addSymbol(table, 1, "MAIN", "code", 96, 4);
    addAttribute(table, 8, "MAIN", "entry");
    addSymbol(table, 2, "LOOP", "code", 96, 8);
    addSymbol(table, 3, "END", "code", 128, 12);
    addSymbol(table, 4, "STR", "data", 128, 13);
    addSymbol(table, 5, "LIST", "data", 144, 2);
    addAttribute(table, 8, "LIST", "entry");
    addSymbol(table, 6, "K", "data", 149, 5);
    addAttribute(table, 8, "K", "entry");
    addSymbol(table, 7, "val1", "external", 0, 0);
    for(i = 0 ; i < 8 ; i++){
        printSymbol(table+i);
    }*/

int startFirstRun(FILE *fp, symbol *symbolTable, binLine *lines, int *ICF, int *DCF, int *tableSize){
    int IC = 100, DC = 0, symbolDecleration, symbolCount = 0, addedLines, error = 1;
    int srcAddressing, destAddressing, numberOfOpperands;
    char command[MAX_COMMAND_NAME_LENGTH], src[MAX_OPPERAND_LENGTH], dest[MAX_OPPERAND_LENGTH];
    char inputLine[MAX_LINE], tempSymbol[MAX_SYMBOL_LENGTH];
    while(fgets(inputLine, MAX_LINE, fp) != NULL){/*2*/
        symbolDecleration = 0;
        if(isSymbolDecleration(inputLine)){/*3*/
            symbolDecleration = 1;/*4*/
        }
        if(isDataDecleration(inputLine)){/*5*/
            if(symbolDecleration){/*6 - add symbol to table*/
                extractSymbol(inputLine, tempSymbol);
                if(!checkValidSymbol(tempSymbol)){
                    fprintf(stderr, "[ERROR]: invalid symbol \"%s\"\n", tempSymbol);
                    error = 0;
                }
                if(findSymbolInTable(symbolTable, symbolCount, tempSymbol) == -1)
                    createSymbol(symbolTable, symbolCount++, tempSymbol, DATA_ATTRIBUTE, 16 * (IC / 16), IC - (16 * (IC / 16)));
                else{
                    fprintf(stderr, "[ERROR]: symbol \"%s\" already exists\n", tempSymbol);
                    error = 0;
                }
            }
            /*7 - add data as bin lines, add DC count according to lines created*/
            addedLines = extractDataFromLine(inputLine, lines+IC-100, symbolTable);
            DC += addedLines;
            IC += addedLines;
        }
        else if(isExternDecleration(inputLine)){/*8*/
            sscanf(inputLine, "%s %s", tempSymbol, tempSymbol);
            if(!checkValidSymbol(tempSymbol))/*check symbol is valid*/
                error = 0;
            if(findSymbolInTable(symbolTable, symbolCount, tempSymbol) == -1)
                createSymbol(symbolTable, symbolCount++, tempSymbol, EXTERNAL_ATTRIBUTE, 0, 0);
            else{
                fprintf(stderr, "[ERROR]: symbol \"%s\" already exists\n", tempSymbol);
                error = 0;
            }
            /*10*/
        }
        else if((!strstr(inputLine, COMMENT_LINE_STRING)) && (!strstr(inputLine, ENTRY_DECLERATION))){/*11 - normal code line*/
            if(symbolDecleration){
                extractSymbol(inputLine, tempSymbol);
                if(!checkValidSymbol(tempSymbol)){
                    fprintf(stderr, "[ERROR]: invalid symbol \"%s\"\n", tempSymbol);
                    error = 0;
                }
                if(findSymbolInTable(symbolTable, symbolCount, tempSymbol) == -1)
                    createSymbol(symbolTable, symbolCount++, tempSymbol, CODE_ATTRIBUTE, 16 * (IC / 16), IC - 16 * (IC / 16));
                else{
                    fprintf(stderr, "[ERROR]: symbol \"%s\" already exists\n", tempSymbol);
                    error = 0;
                }
                skipSymbol(inputLine);
            }
            addedLines = lineDecode(inputLine, command, src, dest, &srcAddressing, &destAddressing, &numberOfOpperands);
            if(!buildCodeLines(numberOfOpperands, lines+IC-100, symbolTable, symbolCount, command, src, srcAddressing,dest, destAddressing)){
                fprintf(stderr, "[ERROR]: code lines were not built, command: \"%s\"\n", command);
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
 * @brief return true if the line is .data or .string
 * 
 * @param inputLine 
 * @return int 
 */
int isDataLine(char *inputLine){
    if(strstr(inputLine, DATA_DECLERATION))
        return 1;
    if(strstr(inputLine, STRING_DECLERATION))
        return 1;
    return 0;
}

int isStringLine(char *inputLine){
    if(strstr(inputLine, STRING_DECLERATION))
        return 1;
    return 0;
}

/**
 * @brief return true if the line has a symbol
 * 
 * @param inputLine 
 * @return int 
 */
int isSymbol(char *inputLine){
    if(strstr(inputLine, LABEL_SYMBOL))
        return 1;
    return 0;
}

void createSymbol(symbol *table, int index, char *symbolName, char *attr ,int base, int offset){
    strcpy((table + (index))->symbol, symbolName);
    (table+index)->baseAddress = base;
    (table+index)->offset = offset;
    strcpy(((table+index)->attributes)[0], attr);
    ((table+index)->attributeCount) = 1;
}

void printSymbol(symbol *s){
    int i;
    printf("{symbol= %s, base= %d, offset= %d, attributes(%d)= ", s->symbol, s->baseAddress, s->offset, s->attributeCount);
    for(i = 0 ; i < (s->attributeCount)-1 ; i++)
        printf("%s, ", s->attributes[i]);
    printf("%s", s->attributes[s->attributeCount-1]);
    printf("}\n");
}

int extractDataFromLine(char *inputLine, binLine *lines, symbol *symbolTable){
    int tempNumber, countLines = 0;
    char tempC;
    char *ptr, tempLine[MAX_LINE];
    if(isStringLine(inputLine)){/*.string*/
        strcpy(tempLine, strstr(inputLine, QUOTATION_SYMBOL) + 1);
        while((*ptr) != QUOTATION_SYMBOL_CHAR){
            tempC = *(ptr++);
            if(tempC == '"')
                tempC = 0;
            createBinaryLine(lines+countLines, 2, MACHINE_CODE_A, tempC);
            countLines++;
        }
        createBinaryLine(lines+countLines, 2, MACHINE_CODE_A, 0);
            countLines++;
    }
    else{/*.data*/
        strcpy(tempLine, strstr(inputLine, DATA_DECLERATION) + 5);
        ptr = strtok(tempLine, COMMA_SYMBOL);
        while(ptr){
            sscanf(ptr, "%d", &tempNumber);
            ptr = strtok(NULL, COMMA_SYMBOL);
            createBinaryLine(lines+countLines, 2, MACHINE_CODE_A, tempNumber);
            countLines++;
        }
    }
    return countLines;
}

int addSymbolToTable(char *inputLine, char *tempSymbol, symbol *symbolTable, int symbolCount, int IC, char *attribute){
    if(findSymbolInTable(symbolTable, symbolCount, tempSymbol) == -1)
        createSymbol(symbolTable, symbolCount++, tempSymbol, attribute, 16 * (IC / 16), IC - 16 * (IC / 16));
    else{
        printf("error: symbol already exists\n");
        return 0;
    }
    return 1;
}

int buildCodeLines(int numberOfOpperands, binLine *lines, symbol *symbolTable, int symbolCount,
    char *command, char *src, int srcAddressing, char *dest, int destAddressing){
    switch (numberOfOpperands)
    {
        case 0:
            return buildMachineCodeLines(lines, symbolCount, symbolTable, command, 0);
        case 1:
            return buildMachineCodeLines(lines, symbolCount, symbolTable, command, 2, dest, destAddressing);
        case 2:
            return buildMachineCodeLines(lines, symbolCount, symbolTable, command, 4, src, srcAddressing, dest, destAddressing);
    }
    return 0;
}

void extractSymbol(char *inputLine, char *symbol){
    char *ptr, tempLine[MAX_LINE];
    strcpy(tempLine, inputLine);
    ptr = strtok(tempLine, LABEL_SYMBOL);
    strcpy(symbol, ptr);
}