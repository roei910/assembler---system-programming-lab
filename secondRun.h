#include <stdio.h>
#include "decodeLine.h"
#include "binaryLine.h"
#include "constants.h"

#if !defined(SECOND_RUN_HEADER)
#define SECOND_RUN_HEADER

/**
 * @brief the function will start second run, will return FALSE if found errors
 * 
 * @param fp 
 * @param extFp 
 * @param symbolTable 
 * @param lines 
 * @param tableSize 
 * @return int 
 */
int startSecondRun(FILE *, FILE *, symbol *, BinaryLine *, int);

/**
 * @brief adds an attribute to the symbol from symbol table array
 * 
 * @param linesCounter 
 * @param table 
 * @param tableSize 
 * @param symbolName 
 * @param attr 
 * @return int 
 */
int addAttribute(int, symbol *, int, char *, char *);

/**
 * @brief extracts the symbol from the entry line to tempsymbol parameter
 * 
 * @param inputLine 
 * @param tempSymbol 
 * @return int 
 */
int extractSymbolFromEntryLine(char *, char *);

/**
 * @brief Get the Symbol From an Opperand to the tempsymbol
 * 
 * @param opperand 
 * @param tempSymbol 
 */
void getSymbolFromOpperand(char *, char *);

/**
 * @brief build the symbol lines to the binline array
 * 
 * @param linesCounter 
 * @param fp 
 * @param lines 
 * @param symbolTable 
 * @param tableSize 
 * @param opperand 
 * @param IC 
 * @return int 
 */
int buildSymbolLines(int, FILE *, BinaryLine *, symbol *, int , char *, int);

/**
 * @brief prints the use of a symbol to the ext file
 * 
 * @param fp 
 * @param extSymbol 
 * @param baseAddress 
 */
void printSymbolExternal(FILE *, symbol *, int);

#endif