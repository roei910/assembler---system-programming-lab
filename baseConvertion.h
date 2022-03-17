#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include "constants.h"
#include "binaryLine.h"

#if !defined(BASE_CONVERTION_HEADER)
#define BASE_CONVERTION_HEADER

void printWord(FILE *, BinaryLine);
void createBinaryLine(BinaryLine *, int, ...);

#endif
