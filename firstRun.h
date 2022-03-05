#include <stdio.h>
#include <ctype.h>
#include <string.h>

#if !defined(DECODE_LINE_HEADER)
#define DECODE_LINE_HEADER
#include "decodeLine.h"
#endif

#define QUOTATION_SYMBOL_CHAR '"'

int startFirstRun(FILE *);
int isSymbolDecleration(char *);
int isDataDecleration(char *);
int isExternDecleration(char *);
int isEntryDecleration(char *);
int checkDecleration(char *, char *);
void extractSymbol(char *, char *);
int isDataLine(char *);
int isSymbol(char *);
void createSymbol(symbol *, int, char *, char * ,int, int);
int addAttribute(symbol *, int, char *, char *);
void printSymbol(symbol *);
int extractDataFromLine(char *, binLine *, symbol *);
int skipString(char *, char *);
int addSymbolToTable(char *, char *, symbol *, int, int, char *);