#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "constants.h"
#include "decodeLine.h"

#if !defined(FIRST_RUN_HEADER)
#define FIRST_RUN_HEADER

#define QUOTATION_SYMBOL_CHAR '"'

/**
 * @brief start a first run of a file fp, returns FALSE if the run was successfull
 * 
 * @param fp 
 * @param symbolTable 
 * @param lines 
 * @param ICF 
 * @param DCF 
 * @param tableSize 
 * @return int 
 */
int startFirstRun(FILE *, symbol *, BinaryLine *, int *, int *, int *);

/**
 * @brief returns true if an inputline contains .string
 * 
 * @param inputLine 
 * @return int 
 */
int isStringLine(char *);

/**
 * @brief Creates a Symbol in the symbol table
 * 
 * @param table 
 * @param index 
 * @param symbolName 
 * @param attr 
 * @param base 
 * @param offset 
 */
void createSymbol(symbol *, int, char *, char *, int, int);

/**
 * @brief creates .string and .data binary lines, returns the amount of lines created
 * 
 * @param inputLine 
 * @param lines 
 * @param line number of line currently read for errors
 * @return int 
 */
int extractDataFromLine(char *, BinaryLine *, int line);

/**
 * @brief build machine code lines to binLine array according to command type
 * 
 * @param linesCounter 
 * @param numberOfOpperands 
 * @param lines 
 * @param symbolTable 
 * @param symbolCount 
 * @param command 
 * @param src 
 * @param srcAddressing 
 * @param dest 
 * @param destAddressing 
 * @return int 
 */
int buildCodeLines(int, int, BinaryLine *, symbol *, int, char *, char *, int, char *, int);

/**
 * @brief extracts a symbol from inputline and copies the symbol to symbol parameter
 * 
 * @param inputLine 
 * @param symbol 
 */
void extractSymbol(char *, char *);

#endif