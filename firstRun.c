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

int startFirstRun(FILE *fp){
    int IC = 100, DC = 0, symbolDecleration, symbolCount = 0, lineCounter=1, i, addedLines;
    char inputLine[MAX_LINE], tempSymbol[MAX_SYMBOL_LENGTH];
    symbol *symbolTable = (symbol *)calloc(MAX_SYMBOLS, sizeof(symbol));
    binLine *lines = (binLine *)calloc(MAX_MACHINE_CODE_LINES, sizeof(binLine));
    while(fgets(inputLine, MAX_LINE, fp) != NULL){/*2*/
        printf("%d. ", lineCounter++);
        symbolDecleration = 0;
        if(isSymbolDecleration(inputLine)){/*3*/
            printf("symbol decleration line");
            symbolDecleration = 1;/*4*/
        }
        if(isDataDecleration(inputLine)){/*5*/
            printf(" ; data decleration line");
            if(symbolDecleration){/*6 - add symbol to table*/
                extractSymbol(inputLine, tempSymbol);
                if(!checkValidSymbol(tempSymbol))/*check symbol is valid*/
                    return 2;
                if(findSymbolInTable(symbolTable, symbolCount, tempSymbol) == -1)
                    createSymbol(symbolTable, symbolCount++, tempSymbol, DATA_DECLERATION, 16 * (IC / 16), IC - 16 * (IC / 16));
                else{
                    printf("error: symbol already exists\n");
                    return 2;
                }
            }
            /*7 - add data as bin lines, add DC count according to lines created*/
            printf("line = %s\n", inputLine);
            addedLines = extractDataFromLine(inputLine, lines+IC-100, symbolTable);
            DC += addedLines;
            IC += addedLines;
        }
        else if(isExternDecleration(inputLine)){/*8*/
            printf(", extern decleration line");
            sscanf(inputLine, "%s %s", tempSymbol, tempSymbol);
            printf("temp symbol = %s\n", tempSymbol);
            if(!checkValidSymbol(tempSymbol))/*check symbol is valid*/
                return 2;
            if(findSymbolInTable(symbolTable, symbolCount, tempSymbol) == -1)
                createSymbol(symbolTable, symbolCount++, tempSymbol, EXTERN_DECLERATION, 0, 0);
            else{
                printf("error: symbol already exists\n");
                return 2;
            }
            /*10*/
        }
        else if((!strstr(inputLine, COMMENT_LINE_STRING)) && (!strstr(inputLine, ENTRY_DECLERATION))){/*11 - normal code line*/
            printf(", something else");
            if(symbolDecleration){
                /*add decleratino with .code*/
                if(!skipString(inputLine, LABEL_SYMBOL))
                    printf("error skipping ********************************\n");
            }
            printf("input line = %s\n", inputLine);
            addedLines = lineDecode(inputLine, lines+IC-100, symbolTable, symbolCount);
            IC += addedLines;
            /*IC++;*/
        }
        puts("");
    }
    printf("symbol count = %d\n", symbolCount);
    for(i = 0 ; i < symbolCount ; i++){
        printSymbol(symbolTable+i);
    }

    printf("IC = %d\n", IC);
    for(i = 0 ; i < IC-100 ; i++){
        printf("%04d\t", i+100);
        printWord(*(lines+i));
    }
    return 0;
}

int isSymbolDecleration(char *inputLine){
    char *cPointer;
    if((cPointer = strstr(inputLine, LABEL_SYMBOL)) != NULL)
        return 1;
    return 0;
}

int isDataDecleration(char *inputLine){
    if(checkDecleration(inputLine, DATA_DECLERATION))
        return 1;
    else if(checkDecleration(inputLine, STRING_DECLERATION))
        return 1;
    return 0;
}

int isExternDecleration(char *inputLine){
    if(checkDecleration(inputLine, EXTERN_DECLERATION))
        return 1;
    return 0;
}

int isEntryDecleration(char *inputLine){
    if(checkDecleration(inputLine, ENTRY_DECLERATION))
        return 1;
    return 0;
}

int checkDecleration(char *inputLine, char *decleration){
    if(strstr(inputLine, decleration) != NULL)
        return 1;
    return 0;
}

void extractSymbol(char *inputLine, char *symbol){
    char *ptr, tempLine[MAX_LINE];
    strcpy(tempLine, inputLine);
    ptr = strtok(tempLine, LABEL_SYMBOL);
    strcpy(symbol, ptr);
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

int addAttribute(symbol *table, int tableSize, char *symbolName, char *attr){
    int index = findSymbolInTable(table, tableSize, symbolName);
    int attrIndex;
    if(index == -1){
        printf("symbol not found\n");
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

void printSymbol(symbol *s){
    int i;
    printf("{symbol= %s, base= %d, offset= %d, attributes(%d)= ", s->symbol, s->baseAddress, s->offset, s->attributeCount);
    for(i = 0 ; i < s->attributeCount ; i++)
        printf("%s, ", s->attributes[i]);
    printf("}\n");
}

int extractDataFromLine(char *inputLine, binLine *lines, symbol *symbolTable){
    int tempNumber, countLines = 0;
    char tempC;
    char *ptr, tempLine[MAX_LINE];
    if(isStringLine(inputLine)){/*.string*/
        strcpy(tempLine, strstr(inputLine, QUOTATION_SYMBOL) + 1);
        while(*ptr > '\n'){
            tempC = *(ptr++);
            if(tempC == '"')
                tempC = 0;
            createBinaryLine(lines+countLines, 2, MACHINE_CODE_A, (int)tempC);
            countLines++;
        }
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

int skipString(char *firstString, char *secondString){
    char *ptr;
    if((ptr = strstr(firstString, secondString))){
        ptr += strlen(secondString);
        while((*ptr) == ' '){
            if(isalnum(*ptr))
                break;
            ptr++;
        }
        strcpy(firstString, ptr);
        return 1;
    }
    return 0;
}

