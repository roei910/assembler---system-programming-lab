#include "constants.h"
#include <string.h>
#include <stdlib.h>

#if !defined(SYMBOL_HEADER)
#define SYMBOL_HEADER

typedef struct symbol Symbol;

Symbol *createSymbolArray(int size);

Symbol *getSymbolAtIndex(Symbol *symbolArray, int index);

char *getAttributesLine(Symbol *symbol, int index);

char *getSymbolName(Symbol *symbol);

int getBaseAddress(Symbol *symbol);

int getOffset(Symbol *);

int getAttributesCount(Symbol *symbol);

void increaseAttributeCount(Symbol *symbol);

/**
 * @brief Creates a Symbol in the symbol table
 * 
 * @param table symbol table
 * @param index current last index
 * @param symbolName name of symbol to create
 * @param attr attribute for symbol
 * @param base base address for symbol
 * @param offset offset from base address for symbol
 */
void createSymbol(Symbol *, int, char *, char *, int, int);


#endif