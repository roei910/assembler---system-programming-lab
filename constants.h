#include <stdlib.h>

#define MAX_LINE 81
#define MAX_SYMBOL_LENGTH 20
#define MAX_ATTRIBUTES 3
#define MAX_ATTRIBUTE_NAME_LENGTH 20
#define MAX_NAME 20
#define NUMBER_OF_REGISTERS 16
#define SIZE_OF_WORD 20
#define MAX_MACHINE_CODE_LINES 8192
#define DATA_DECLERATION ".data"
#define STRING_DECLERATION ".string"
#define EXTERN_DECLERATION ".extern"
#define ENTRY_DECLERATION ".entry"

/*
enum opcode{mov=0, cmp, add, sub=add, lea=4,
            clr, not=clr, inc=clr, dec=clr,
            jmp=9, bne=jmp, jsr=jmp,
            red=12, prn, rts, stop};

enum funct{ADD=10,CLR=ADD,JMP=10,
            SUB=11,NOT=11,BNE=11,
            INC=12,JSR=INC,DEC=13};
            */

typedef struct{
    char label[MAX_NAME];
    char commandName[MAX_NAME];
    char firstOpperand[MAX_NAME];
    char secondOpperand[MAX_NAME];
}Command;

typedef struct{/*represents a row in the symbol table*/
    char symbol[MAX_SYMBOL_LENGTH];
    int baseAddress;
    int offset;
    int attributeCount;
    char attributes[MAX_ATTRIBUTES][MAX_ATTRIBUTE_NAME_LENGTH];
}symbol;

typedef struct{/*binary line*/
    unsigned int IC;
    union MachineCode{
        unsigned int code:SIZE_OF_WORD;
        
        struct 
        {
            unsigned int addressingDest:2;
            unsigned int dest:4;
            unsigned int addressingSrc:2;
            unsigned int src:4;
            unsigned int funct:4;
            unsigned int A:4;
        }word;
        
        struct 
        {
            unsigned int E:4;
            unsigned int D:4;
            unsigned int C:4;
            unsigned int B:4;
            unsigned int A:4;
        }wordPrint;

    } machineCode;
    /*unsigned int machineCode:SIZE_OF_WORD;*/
}binLine;