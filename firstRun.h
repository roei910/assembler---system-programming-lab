#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "constants.h"
#include "decodeLine.h"

#define QUOTATION_SYMBOL_CHAR '"'

int startFirstRun(FILE *, symbol *, BinaryLine *, int *, int *, int *);
int isStringLine(char *);
void createSymbol(symbol *, int, char *, char *, int, int);
int extractDataFromLine(char *, BinaryLine *);
int buildCodeLines(int, int, BinaryLine *, symbol *, int, char *, char *, int, char *, int);
void extractSymbol(char *, char *);