#include <stdio.h>
#include <ctype.h>
#include <string.h>

#if !defined(DECODE_LINE_HEADER)
#define DECODE_LINE_HEADER
#include "decodeLine.h"
#endif

#define QUOTATION_SYMBOL_CHAR '"'

int startFirstRun(FILE *, symbol *, binLine *, int *, int *, int *);
void extractSymbol(char *, char *);
void createSymbol(symbol *, int, char *, char * ,int, int);
int buildCodeLines(int, binLine *, symbol *, int, char *, char *, int, char *, int);
int extractDataFromLine(char *, binLine *, symbol *);

/*
int isDataLine(char *);
int isSymbol(char *);
void printSymbol(symbol *);
int addSymbolToTable(char *, char *, symbol *, int, int, char *);
*/