#include <stdio.h>

#if !defined(DECODE_LINE_HEADER)
#define DECODE_LINE_HEADER
#include "decodeLine.h"
#endif

int startSecondRun(FILE *, symbol *, binLine *, int);
int writeSymbolLine(binLine *, int, int);
int isEntryDecleration(char *);
int addAttribute(symbol *, int, char *, char *);
int extractSymbolFromEntryLine(char *, char *);