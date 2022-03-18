#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "constants.h"

#if !defined(PRE_COMPILER_HEADER)
#define PRE_COMPILER_HEADER

#define MAX_MACRO_NAME 20
#define MAX_MACRO_LENGTH 6 /*max lines for a macro decleration*/

typedef struct node{
    char name[MAX_MACRO_NAME];
    char macroContent[MAX_MACRO_LENGTH][MAX_LINE]; 
    int lines;
    struct node *next;
}macroNode;

/**
 * @brief starts to replace all macro on file fp
 * 
 * @param fp 
 * @param fileName 
 * @return int 
 */
int replaceAllMacro(FILE *, char *);

/**
 * @brief Create a New Node of macro type
 * 
 * @param file 
 * @param macroName 
 * @param node 
 */
void createNewNode(FILE *, char *, macroNode *);

/**
 * @brief finds if a macro name is in the macro node list
 * 
 * @param macroName 
 * @param macro 
 * @return macroNode* 
 */
macroNode *findMacro(char *, macroNode *);

/**
 * @brief checks if an inputline contains a macro decleration
 * 
 * @param inputLine 
 * @return char* 
 */
char *isMacro(char *);

/**
 * @brief inserts a new macro to the macro node list
 * 
 * @param macro 
 * @param newNode 
 */
void insertMacro(macroNode **, macroNode *);

/**
 * @brief prints the macro to the new file created to replace the macro callback
 * 
 * @param macro 
 * @param fp 
 */
void fprintMacro(macroNode *, FILE *);

/**
 * @brief write the first line of the file
 * 
 * @param fp 
 * @param fileName 
 */
void writeNewFileName(FILE *, char *);

#endif