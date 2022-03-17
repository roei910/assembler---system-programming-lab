#include <stdio.h>
#include "decodeLine.h"
#include "binaryLine.h"
#include "constants.h"

int startSecondRun(FILE *, FILE *, symbol *, BinaryLine *, int);
int addAttribute(int, symbol *, int, char *, char *);
int extractSymbolFromEntryLine(char *, char *);
void getSymbolFromOpperand(char *, char *);
int buildSymbolLines(int, FILE *, BinaryLine *, symbol *, int , char *, int);
void printSymbolExternal(FILE *, symbol *, int);