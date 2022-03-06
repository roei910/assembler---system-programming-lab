#include <stdio.h>

#if !defined(DECODE_LINE_HEADER)
#define DECODE_LINE_HEADER
#include "decodeLine.h"
#endif

int startSecondRun(FILE *, FILE *, symbol *, binLine *, int);
int isEntryDecleration(char *);
int addAttribute(symbol *, int, char *, char *);
int extractSymbolFromEntryLine(char *, char *);
void getSymbolFromOpperand(char *, char *);
/*void buildSymbolLines(binLine *, int, int, int);*/
int buildSymbolLines(FILE *, binLine *, symbol *, int , char *, int);
void printSymbolExternal(FILE *, symbol *, int);