#include <stdio.h>
#include "preCompiler.h"
#include "firstRun.h"
#include "secondRun.h"

void runProgram(char *);
void preCompiler(char *);
void buildOutPutFiles(char *, binLine *, symbol *, int, int, int);
void printSymbolEntry(FILE *, symbol *, int);
