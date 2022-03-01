#include "firstRun.h"

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



int checkValidSymbol(char *symbol){
    while(*symbol != '\0'){
        if(!isalnum(*symbol))
            return 0;
        symbol++;
    }
    return 1;
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