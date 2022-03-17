#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "constants.h"

#define MAX_MACRO_NAME 20
#define MAX_MACRO_LENGTH 6 /*max lines for a macro decleration*/

typedef struct node{
    char name[MAX_MACRO_NAME];
    char macroContent[MAX_MACRO_LENGTH][MAX_LINE]; 
    int lines;
    struct node *next;
}macroNode;

int replaceAllMacro(FILE *, char *);
void createNewNode(FILE *, char *, macroNode *);
macroNode *findMacro(char *, macroNode *);
char *isMacro(char *);
void insertMacro(macroNode **, macroNode *);
void fprintMacro(macroNode *, FILE *);
void writeNewFileName(FILE *, char *);