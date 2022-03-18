#if !defined(CONSTANTS_HEADER)
#define CONSTANTS_HEADER

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
#define CODE_ATTRIBUTE "code"
#define EXTERNAL_ATTRIBUTE "external"
#define DATA_ATTRIBUTE "data"
#define ENTRY_ATTRIBUTE "entry"
#define MAX_SYMBOLS 50

typedef struct{/*represents a row in the symbol table*/
    char symbol[MAX_SYMBOL_LENGTH];
    int baseAddress;
    int offset;
    int attributeCount;
    char attributes[MAX_ATTRIBUTES][MAX_ATTRIBUTE_NAME_LENGTH];
}symbol;

#endif
