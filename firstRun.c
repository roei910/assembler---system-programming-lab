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
    int /*IC = 100, DC = 0, */symbolDecleration, symbolCount = 0, i;
    char inputLine[MAX_LINE], tempSymbol[MAX_SYMBOL_LENGTH];
    symbol *table = (symbol *)malloc(sizeof(symbol));
    while(fgets(inputLine, MAX_LINE, fp) != NULL){/*2*/
        symbolDecleration = 0;
        if(isSymbolDecleration(inputLine))/*3*/
            symbolDecleration = 1;/*4*/
        if(isDataDecleration(inputLine)){/*5*/
            if(symbolDecleration){/*6*/
                extractSymbol(inputLine, tempSymbol);
                if(!checkValidSymbol(tempSymbol))/*check symbol is valid*/
                    return 2;
                for(i = 0 ; i < symbolCount; i++){
                    if((table+i)->symbol == tempSymbol)
                        return 1;/*error - symbol already exist*/
                }
                table = (symbol *)realloc(table, symbolCount+1 * sizeof(symbol));
                strcpy((table + (symbolCount))->symbol, tempSymbol);
                /*add .data to the symbol*/
                /**/
                printf("symbol: %s\n", (table+symbolCount)->symbol);
                symbolCount++;
            }
            /*7 - add data*/
        }
        else if(isExternDecleration(inputLine)){/*8*/
            /*10*/
        }
        else{/*11*/

        }
    }
    return 0;
}

int isSymbolDecleration(char *inputLine){
    char *cPointer;
    if((cPointer = strstr(inputLine, ":")) != NULL)
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
    char *ptr;
    char delim[2] = ":";
    ptr = strtok(inputLine, delim);
    strcpy(symbol, ptr);
}

/**
 * @brief return true if the line is .data or .string
 * 
 * @param inputLine 
 * @return int 
 */
int isDataLine(char *inputLine){
    if(strstr(inputLine, DATA_COMMAND))
        return 1;
    if(strstr(inputLine, STRING_COMMAND))
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

void addSymbol(symbol *table, int index, char *symbolName, char *attr ,int base, int offset){
    strcpy((table+index)->symbol, symbolName);
    (table+index)->baseAddress = base;
    (table+index)->offset = offset;
    strcpy(((table+index)->attributes)[0], attr);
    ((table+index)->attributeCount)++;
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

int findSymbolInTable(symbol *table, int tableSize, char *symbolName){
    int i;
    for(i = 0 ; i < tableSize ; i++)
        if(!strcmp((table+i)->symbol, symbolName))
            return i;
    return -1;
}

int isExtern(char *inputLine){
    if(strstr(inputLine, EXTERN_COMMAND))
        return 1;
    return 0;
}

int isEntry(char *inputLine){
    if(strstr(inputLine, ENTRY_COMMAND))
        return 1;
    return 0;
}

void printSymbol(symbol *s){
    int i;
    printf("{symbol= %s, base= %d, offset= %d, attributes(%d)= ", s->symbol, s->baseAddress, s->offset, s->attributeCount);
    for(i = 0 ; i < s->attributeCount ; i++)
        printf("%s, ", s->attributes[i]);
    printf("}\n");
}

