#if !defined(FIRST_RUN_HEADER)
#define FIRST_RUN_HEADER

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "constants.h"
#include "decodeLine.h"
#include "symbol.h"

#define QUOTATION_SYMBOL_CHAR '"'

/**
 * @brief start a first run of a file fp, returns FALSE if the run was successfull
 * 
 * @param fp FILE pointer to be read after macro
 * @param symbolTable symbol table
 * @param lines binary lines of assembly line
 * @param ICF instruction lines in assembly file
 * @param DCF data lines in assembly file
 * @param tableSize size of symbol table
 * @return int 
 */
int startFirstRun(FILE *, Symbol *, BinaryLine *, int *, int *, int *);

/**
 * @brief returns true if an inputline contains .string
 * 
 * @param inputLine input line to check
 * @return int 
 */
int isStringLine(char *);

/**
 * @brief creates .string and .data binary lines, returns the amount of lines created
 * 
 * @param inputLine input line to be checked
 * @param lines binary lines to be created
 * @param line number of line currently read for errors
 * @return int 
 */
int extractDataFromLine(char *, BinaryLine *, int line);

/**
 * @brief build machine code lines to binLine array according to command type
 * 
 * @param linesCounter current line of assembly file
 * @param numberOfOpperands number of opperands for the current command name
 * @param lines binary lines
 * @param symbolTable symbol table
 * @param symbolCount symbol table size
 * @param command command name
 * @param src source opperand
 * @param srcAddressing type of addressing for source
 * @param dest destination opperand
 * @param destAddressing type of addressing for destination
 * @return int 
 */
int buildCodeLines(int, int, BinaryLine *, Symbol *, int, char *, char *, int, char *, int);

/**
 * @brief extracts a symbol from inputline and copies the symbol to symbol parameter
 * 
 * @param inputLine input line to extract symbol from
 * @param symbol destination for symbol
 */
void extractSymbol(char *, char *);

#endif