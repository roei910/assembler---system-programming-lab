#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include "constants.h"
#include "binaryLine.h"

#if !defined(DECODE_LINE_HEADER)
#define DECODE_LINE_HEADER

#define MAX_OPPERAND_LENGTH 30
#define MAX_COMMAND_NAME_LENGTH 20
#define MAX_VARIABLE_NAME_LENGTH 20
#define MAX_LABEL_LENGTH 20
#define SPACE_SYMBOL " "
#define COMMENT_LINE_SYMBOL ';'
#define COMMENT_LINE_STRING ";"
#define INSTRUCTION_LINE_SYMBOL '.'
#define COMMA_SYMBOL ","
#define QUOTATION_SYMBOL "\""
#define LABEL_SYMBOL ":"
#define HASHTAG_SYMBOL "#"
#define LEFT_SQUARE_BRACKET "["
#define RIGHT_SQUARE_BRACKET "]"
#define REGISTER_R_SYMBOL "r"
#define COMMAND_MOV "mov"
#define COMMAND_CMP "cmp"
#define COMMAND_ADD "add"
#define COMMAND_SUB "sub"
#define COMMAND_LEA "lea"
#define COMMAND_CLR "clr"
#define COMMAND_NOT "not"
#define COMMAND_INC "inc"
#define COMMAND_DEC "dec"
#define COMMAND_JMP "jmp"
#define COMMAND_BNE "bne"
#define COMMAND_JSR "jsr"
#define COMMAND_RED "red"
#define COMMAND_PRN "prn"
#define COMMAND_RTS "rts"
#define COMMAND_STOP "stop"
#define MACHINE_CODE_A 2
#define MACHINE_CODE_R 1
#define MACHINE_CODE_E 0

/**
 * @brief decode a single line from the file and return a number of lines to be added to the file
 * 
 * @param line current line from file for errors
 * @param inputLine the input line to be decode
 * @param command destination for command name
 * @param src destination for source opperand
 * @param dest destination for destination opperand
 * @param srcAddressing destination for type of addressing for source
 * @param destAddressing destination for type of addressing for destination
 * @param numberOfOpperands destination for number of opperands for command name
 * @return int number of lines to be written
 */
int lineDecode(int, char *, char *, char *, char *, int *, int *, int *);

/**
 * @brief return the number of opperands in a single input line
 * 
 * @param inputLine input line to be decoded
 * @param command destination for command name
 * @param src destination for source opperand
 * @param dest destination for destination opperand
 * @param line number of line for error output
 * @return int number of opperands
 */
int decodeInstructionLine(char *, char *, char *, char *, int);

/**
 * @brief check the opperand type of a given opperand
 * 0 - number
 * 1 - symbol and register
 * 2 - register
 * 3 - symbol
 * -1 - invalid
 * 
 * @param opperand the opperand to be checked
 * @return int 
 */
int checkOpperandType(char *);

/**
 * @brief return true if an opperand is a number
 * 
 * @param opperand the opperand to be checked
 * @return int 
 */
int isNumber(char *);

/**
 * @brief returns true if an opperand is a register without checking if the register number is valid (0 to 15)
 * 
 * @param opperand the opperand to be checked
 * @return int 
 */
int isRegister(char *);

/**
 * @brief returns true if an opperand is a symbol and register
 * 
 * @param opperand the opperand to be checked
 * @return int 
 */
int isIndexOpperand(char *);

/**
 * @brief Get the Number Of Opperands from a command name, returns -1 for wrong command name
 * 
 * @param command the command name to be checked
 * @return int 
 */
int getNumberOfOpperands(char *);

/**
 * @brief checks if a command with two opperands is valid
 * 
 * @param command command name
 * @param src source opperand
 * @param srcAddressing destination for source addressing
 * @param dest destination opperand
 * @param destAddressing destination for destination addressing
 * @return int 0 - FALSE
 */
int checkValidCommandTwoOpperands(char *, char *, int *, char *, int *);

/**
 * @brief checks if a command with one opperand is valid
 * 
 * @param command comamnd name
 * @param dest destination opperand
 * @param destAddressing destination for destination addressing
 * @return int 
 */
int checkValidCommandOneOpperand(char *, char *, int *);

/**
 * @brief Get the Funct of a command name
 * 
 * @param command command name to be checked
 * @return int FUNCT of command
 */
int getFunct(char *);

/**
 * @brief Get the Opcode of a command name
 * 
 * @param command command name to be checked
 * @return int OPCODE of command
 */
int getOpcode(char *);

/**
 * @brief creates binary lines according to the type of command, a symbol line doesnt get a value
 * 
 * @param linesCounter number of line in file for errors
 * @param lines binary lines
 * @param symbolCount symbol table size
 * @param symbolTable symbol table
 * @param command name of command
 * @param arguments number of arguments
 * @param ... 
 * @return int 
 */
int buildMachineCodeLines(int, BinaryLine *, int, symbol *, char *, int, ...);

/**
 * @brief Get the Number From an Opperand
 * 
 * @param opperand the opperand to be checked
 * @return int 
 */
int getNumberFromOpperand(char *);

/**
 * @brief Get the Reg From an opperand
 * 
 * @param opperand the opperand to be checked
 * @return int 
 */
int getRegFromOpperand(char *);

/**
 * @brief checks if a symbol is valid (alpha numeric characters only)
 * 
 * @param symbol thr symbol to be checked
 * @return int 
 */
int checkValidSymbol(char *);

/**
 * @brief returns true if an inputline is a symbol decleration
 * 
 * @param inputLine inputline to be checked
 * @return int 
 */
int isSymbolDecleration(char *);

/**
 * @brief returns true if an inputline is a data decleration
 * 
 * @param inputLine inputline to be checked
 * @return int 
 */
int isDataDecleration(char *);

/**
 * @brief returns true if an inputline is an extern decleration
 * 
 * @param inputLine inputline to be checked
 * @return int 
 */
int isExternDecleration(char *);

/**
 * @brief skip a symbol in the beggining of an inputline
 * 
 * @param inputLine inputline to be checked
 * @return int 
 */
int skipSymbol(char *);

/**
 * @brief checks if a decleration string is in the inputline
 * 
 * @param inputLine inputline to be checked
 * @param decleration type of decleration to be in inputline
 * @return int 
 */
int checkDecleration(char *, char *);

/**
 * @brief returns true if a symbol is in the symbol table
 * 
 * @param table symbole table
 * @param tableSize size of symbol table
 * @param symbolName name of symbol to be found
 * @return int 
 */
int findSymbolInTable(symbol *, int, char *);

/**
 * @brief returns true if an inputline is an empty line
 * 
 * @param inputLine inputline to be checked
 * @return int 
 */
int isEmptyLine(char *);

/**
 * @brief returns true if a line is a comment line
 * 
 * @param inputLine inputline to be checked
 * @return int 
 */
int isCommentLine(char *);

/**
 * @brief returns true if an inputline is an entry decleration line
 * 
 * @param inputLine inputline to be checked
 * @return int 
 */
int isEntryDecleration(char *);

#endif
