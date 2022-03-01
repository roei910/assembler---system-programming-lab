#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "decodeLine.h"

#if !defined(CONSTANTS_HEADER)
#define CONSTANTS_HEADER
#include "constants.h"
#endif


#define DATA_DECLERATION ".data"
#define STRING_DECLERATION ".string"
#define EXTERN_DECLERATION ".extern"
#define ENTRY_DECLERATION ".entry"
#define MAX_SYMBOL_LENGTH 20

int startFirstRun(FILE *);
int isSymbolDecleration(char *);
int isDataDecleration(char *);
int isExternDecleration(char *);
int isEntryDecleration(char *);
int checkDecleration(char *, char *);
void extractSymbol(char *, char *);
int checkValidSymbol(char *);