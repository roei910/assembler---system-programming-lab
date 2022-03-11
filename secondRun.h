#include <stdio.h>

#if !defined(DECODE_LINE_HEADER)
#define DECODE_LINE_HEADER
#include "decodeLine.h"
#endif

int startSecondRun(FILE *, FILE *, symbol *, binLine *, int);
void getSymbolFromOpperand(char *, char *);
int extractSymbolFromEntryLine(char *, char *);
int addAttribute(int, symbol *, int, char *, char *);
int buildSymbolLines(int, FILE *, binLine *, symbol *, int , char *, int);
void printSymbolExternal(FILE *, symbol *, int);