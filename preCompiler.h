#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if !defined(CONSTANTS_HEADER)
#define CONSTANTS_HEADER
#include "constants.h"
#endif

#define MAX_MACRO_NAME 20
#define MAX_MACRO_LENGTH 6

typedef struct node{
    char name[MAX_MACRO_NAME];
    char macroContent[MAX_MACRO_LENGTH][MAX_LINE]; 
    int startingLine;
    int lines;
    struct node *next;
}MacroNode;

void writeNewFileName(FILE *, char *);
void fprintMacro(MacroNode *, FILE *);
void insertMacro(MacroNode **, MacroNode*);
char *isMacro(char *);
MacroNode *findMacro(char *, MacroNode *);
MacroNode *createNewNode(char *, FILE **);
int replaceAllMacro(FILE *, char *);