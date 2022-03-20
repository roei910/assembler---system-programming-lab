#if !defined(PRE_ASSEMBLER_HEADER)
#define PRE_ASSEMBLER_HEADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "constants.h"
#include "macroNode.h"

/**
 * @brief starts to replace all macro on file fp
 * 
 * @param fp FILE pointer to original assembly file
 * @param fileName new file name to create new file after macro
 * @return int 
 */
int replaceAllMacro(FILE *, char *);

/**
 * @brief finds if a macro name is in the macro node list
 * 
 * @param macroName the name of the macro
 * @param macro linked list with all macros
 * @return pointer to the macro node, NULL if not found
 */
MacroNode *findMacro(char *, MacroNode *);

/**
 * @brief checks if an inputline contains a macro decleration
 * 
 * @param inputLine the line to be read
 * @return char* pointer to start of macro decleration
 */
char *isMacro(char *);

/**
 * @brief inserts a new macro to the macro node list
 * 
 * @param macro macro linked list
 * @param newNode node to be added
 */
void insertMacro(MacroNode **, MacroNode *);

/**
 * @brief prints the macro to the new file created to replace the macro callback
 * 
 * @param macro macro linked list
 * @param fp pointer to the file to write after macro
 */
void fprintMacro(MacroNode *, FILE *);

/**
 * @brief write the first line of the file
 * 
 * @param fp pointer to the file to write after macro
 * @param fileName new file name to write in beggining of file
 */
void writeNewFileName(FILE *, char *);

#endif