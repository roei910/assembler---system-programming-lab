#include <stdio.h>
#include "preCompiler.h"
#include "firstRun.h"
#include "secondRun.h"

int runProgram(char *);
int preCompiler(char *);
int buildOutPutFiles(char *, binLine *, symbol *, int, int, int);
void printSymbolEntry(FILE *, symbol *, int);