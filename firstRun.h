#include <stdio.h>
#include <ctype.h>
#include <string.h>

#if !defined(DECODE_LINE_HEADER)
#define DECODE_LINE_HEADER
#include "decodeLine.h"
#endif

int startFirstRun(FILE *);
int checkValidSymbol(char *);
int isSymbolDecleration(char *);
int isDataDecleration(char *);
int isExternDecleration(char *);
int isEntryDecleration(char *);
int checkDecleration(char *, char *);
void extractSymbol(char *, char *);
int isDataLine(char *);
int isSymbol(char *);
void addSymbol(symbol *, int, char *, char * ,int, int);
int addAttribute(symbol *, int, char *, char *);
int findSymbolInTable(symbol *, int, char *);
int isExtern(char *);
int isEntry(char *);
void printSymbol(symbol *);