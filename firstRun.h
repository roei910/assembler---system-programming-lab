#include <stdio.h>
#include <ctype.h>
#include <string.h>

#if !defined(DECODE_LINE_HEADER)
#define DECODE_LINE_HEADER
#include "decodeLine.h"
#endif

#define QUOTATION_SYMBOL_CHAR '"'

int startFirstRun(FILE *, symbol *, binLine *, int *, int *, int *);
int isDataLine(char *);
int isSymbol(char *);
void createSymbol(symbol *, int, char *, char * ,int, int);
void printSymbol(symbol *);
int extractDataFromLine(char *, binLine *, symbol *);
int addSymbolToTable(char *, char *, symbol *, int, int, char *);
void extractSymbol(char *, char *);