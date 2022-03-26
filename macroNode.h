#if !defined(MACRO_NODE_HEADER)
#define MACRO_NODE_HEADER

#include "stdio.h"

#define MAX_MACRO_NAME 20
#define MAX_MACRO_LENGTH 6 /*max lines for a macro decleration*/

/**
 * @brief macroNode object
 * object includes:
 * 1. name - array of chars
 * 2. macroContent - 2d array of chars
 * 3. lines - int, number of lines in the macroNode
 * 4. next - the next object of node
 * 
 */
typedef struct macroNode MacroNode;

/**
 * @brief creates a new macroNode
 * 
 * @return MacroNode* 
 */
MacroNode *newMacroNodeList();

/**
 * @brief Get the Macro Node object name
 * 
 * @param macroNode pointer to macroNode object
 * @return char* 
 */
char *getMacroNodeName(MacroNode *);

/**
 * @brief Get the Macro Node line At given Index
 * 
 * @param macroNode pointer to macroNode object
 * @param index index of macroNode content 2d array
 * @return char* 
 */
char *getMacroNodeContentAtIndex(MacroNode *, int );

/**
 * @brief Create a New Node of macro type
 * 
 * @param file FILE pointer to original assembly file
 * @param macroName destination for macro name
 * @param node destination for new node of macro
 */
void createNewNode(FILE *, char *, MacroNode *);

/**
 * @brief Get the Next object of macroNode
 * 
 * @param macroNode pointer to current macroNode object
 * @return MacroNode* 
 */
MacroNode *getNext(MacroNode *);

/**
 * @brief Set the Next macroNode object
 * 
 * @param macroNode current macroNode object
 * @param newNode next object to be set
 */
void setNextNode(MacroNode *, MacroNode *);

/**
 * @brief Get the Macro amount of line in a macroNode object
 * 
 * @param macroNode pointer to macroNode object
 * @return int 
 */
int getMacroLines(MacroNode *);


#endif