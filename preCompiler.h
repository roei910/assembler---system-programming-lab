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

int replaceAllMacro(FILE *, char *);
MacroNode *findMacro(char *, MacroNode *);
char *isMacro(char *);
void insertMacro(MacroNode **, MacroNode*);
void fprintMacro(MacroNode *, FILE *);
MacroNode *createNewNode(char *, FILE **);
void writeNewFileName(FILE *, char *);