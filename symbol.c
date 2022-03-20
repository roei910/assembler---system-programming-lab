#include "symbol.h"

struct symbol{/*represents a row in the symbol table*/
    char symbolName[MAX_SYMBOL_LENGTH];
    int baseAddress;
    int offset;
    int attributeCount;
    char attributes[MAX_ATTRIBUTES][MAX_ATTRIBUTE_NAME_LENGTH];
};

Symbol *createSymbolArray(int size){
    return (Symbol *)calloc(size, sizeof(Symbol));
}

Symbol *getSymbolAtIndex(Symbol *symbolArray, int index){
    return symbolArray+index;
}

char *getAttributesLine(Symbol *symbol, int index){
    return symbol->attributes[index];
}

char *getSymbolName(Symbol *symbol){
    return symbol->symbolName;
}

int getBaseAddress(Symbol *symbol){
    return symbol->baseAddress;
}

int getOffset(Symbol *symbol){
    return symbol->offset;
}

int getAttributesCount(Symbol *symbol){
    return symbol->attributeCount;
}

void increaseAttributeCount(Symbol *symbol){
    symbol->attributeCount++;
}

void createSymbol(Symbol *table, int index, char *newSymbolName, char *attr ,int base, int offset){
    strcpy((table + (index))->symbolName, newSymbolName);
    (table+index)->baseAddress = base;
    (table+index)->offset = offset;
    strcpy(((table+index)->attributes)[0], attr);
    ((table+index)->attributeCount) = 1;
}