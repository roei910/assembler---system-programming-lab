#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>

#if !defined(CONSTANTS_HEADER)
#define CONSTANTS_HEADER
#include "constants.h"
#endif

#if !defined(BASE_CONVERTION_HEADER)
#define BASE_CONVERTION_HEADER
#include "baseConvertion.h"
#endif



#define MAX_OPPERAND_LENGTH 30
#define MAX_COMMAND_NAME_LENGTH 20
#define MAX_VARIABLE_NAME_LENGTH 20
#define MAX_LABEL_LENGTH 20
#define SPACE_SYMBOL " "
#define COMMENT_LINE_SYMBOL ';'
#define COMMENT_LINE_STRING ";"
#define INSTRUCTION_LINE_SYMBOL '.'
#define COMMA_SYMBOL ","
#define LABEL_SYMBOL ":"
#define HASHTAG_SYMBOL "#"
#define LEFT_SQUARE_BRACKET "["
#define RIGHT_SQUARE_BRACKET "]"
#define REGISTER_R_SYMBOL "r"
#define ENTRY_COMMAND ".entry"
#define EXTERN_COMMAND ".extern"
#define DATA_COMMAND ".data"
#define STRING_COMMAND ".string"
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
#define MACHINE_CODE_A 18
#define MACHINE_CODE_R 17
#define MACHINE_CODE_E 16

void lineDecode();
int isDataLine(char *);
int isSymbol(char *);
void addSymbol(symbol *, int , char *, char * ,int , int );
int addAttribute(symbol *, int , char *, char *);
int findSymbolInTable(symbol *, int , char *);
int isExtern(char *);
int isEntry(char *);
void printSymbol(symbol *);
int checkValidSymbol(char *);
int decodeInstructionLine(char *, char *, char *, char *);
int checkOpperandType(char *);
int numberOfOpperands(char *);
int isRegister(char *);
int isIndexOpperand(char *);
int isNumber(char *);
int checkValidOpperandsCommand(char *, char *, char *, char *);
int getFunct(char *);
int getOpcode(char *);
int checkValidCommandTwoOpperands(char *,char *,int *,char *,int *);
int checkValidCommandOneOpperand(char *, char *, int *);
int buildMachineCodeLines(binLine *, int , char *, int , ...);
int getNumberFromOpperand(char *);
int getRegFromOpperand(char *);
int writeSymbolLine(binLine *, int, int);
int writeSymbolLine(binLine *, int , int );