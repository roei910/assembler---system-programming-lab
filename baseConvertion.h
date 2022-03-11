#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#if !defined(CONSTANTS_HEADER)
#define CONSTANTS_HEADER
#include "constants.h"
#endif

void printWord(FILE *, binLine);
void createBinaryLine(binLine *, int, ...);