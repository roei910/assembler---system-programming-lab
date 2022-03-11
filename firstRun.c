#include "firstRun.h"

int startFirstRun(FILE *fp, symbol *symbolTable, binLine *lines, int *ICF, int *DCF, int *tableSize){
    int IC = 100, DC = 0, symbolDecleration, symbolCount = 0, addedLines, error = 1;
    int srcAddressing, destAddressing, numberOfOpperands;
    char command[MAX_COMMAND_NAME_LENGTH], src[MAX_OPPERAND_LENGTH], dest[MAX_OPPERAND_LENGTH];
    char inputLine[MAX_LINE], tempSymbol[MAX_SYMBOL_LENGTH];
    while(fgets(inputLine, MAX_LINE, fp) != NULL){
        symbolDecleration = 0;
        if(isSymbolDecleration(inputLine)){
            symbolDecleration = 1;
        }
        if(isDataDecleration(inputLine)){
            if(symbolDecleration){
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

/*
int isDataLine(char *inputLine){
    if(strstr(inputLine, DATA_DECLERATION))
        return 1;
    if(strstr(inputLine, STRING_DECLERATION))
        return 1;
    return 0;
}*/

int isStringLine(char *inputLine){
    if(strstr(inputLine, STRING_DECLERATION))
        return 1;
    return 0;
}

/*
int isSymbol(char *inputLine){
    if(strstr(inputLine, LABEL_SYMBOL))
        return 1;
    return 0;
}
*/

void createSymbol(symbol *table, int index, char *symbolName, char *attr ,int base, int offset){
    strcpy((table + (index))->symbol, symbolName);
    (table+index)->baseAddress = base;
    (table+index)->offset = offset;
    strcpy(((table+index)->attributes)[0], attr);
    ((table+index)->attributeCount) = 1;
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

/*
void printSymbol(symbol *s){
    int i;
    printf("{symbol= %s, base= %d, offset= %d, attributes(%d)= ", s->symbol, s->baseAddress, s->offset, s->attributeCount);
    for(i = 0 ; i < (s->attributeCount)-1 ; i++)
        printf("%s, ", s->attributes[i]);
    printf("%s", s->attributes[s->attributeCount-1]);
    printf("}\n");
}*/

/*
int addSymbolToTable(char *inputLine, char *tempSymbol, symbol *symbolTable, int symbolCount, int IC, char *attribute){
    if(findSymbolInTable(symbolTable, symbolCount, tempSymbol) == -1)
        createSymbol(symbolTable, symbolCount++, tempSymbol, attribute, 16 * (IC / 16), IC - 16 * (IC / 16));
    else{
        fprintf(stderr, "[ERROR]: symbol \"%s\" already exists\n", tempSymbol);
        return 0;
    }
    return 1;
}*/