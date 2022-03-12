#include <stdio.h>
#include <ctype.h>
#include <string.h>

#if !defined(DECODE_LINE_HEADER)
#define DECODE_LINE_HEADER
#include "decodeLine.h"
#endif

#define QUOTATION_SYMBOL_CHAR '"'

int startFirstRun(FILE *, symbol *, binLine *, int *, int *, int *);
int isStringLine(char *);
void createSymbol(symbol *, int, char *, char *, int, int);
int extractDataFromLine(char *, binLine *);
int buildCodeLines(int, int, binLine *, symbol *, int, char *, char *, int, char *, int);
void extractSymbol(char *, char *);