#include <stdio.h>
#include "preAssembler.h"
#include "firstRun.h"
#include "secondRun.h"

#if !defined(ASSEMBLER_HEADER)
#define ASSEMBLER_HEADER

/**
 * @brief start to run assembler program for each file received
 * 
 * @param argc 
 * @param argv 
 */
void startAssembler(int, char **);

/**
 * @brief run assembler for a file name, returns true for successfull run
 * 
 * @param fileName the file to be assembled
 */
int runProgram(char *);

/**
 * @brief run the file through preAssembler program, return true if the preAssembler was successfull
 * 
 * @param fileName the file name to be run preAssembler
 */
int preAssembler(char *);

/**
 * @brief build the output files after first and second run was successfull
 * 
 * @param fileName file name to be created
 * @param lines binary lines read from files
 * @param symbolTable symbol table from the file
 * @param symbolTableSize size of symbol table
 * @param ICF instruction count of the read file
 * @param DCF data count of the read file
 */
int buildOutPutFiles(char *, BinaryLine *, symbol *, int, int, int);

/**
 * @brief print an entry symbols according to the instructions
 * 
 * @param fp file to be written to
 * @param symbolTable symbols read from the file
 * @param symbolTableSize size of symbol table array
 */
void printSymbolEntry(FILE *, symbol *, int);

#endif