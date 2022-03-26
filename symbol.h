#if !defined(SYMBOL_HEADER)
#define SYMBOL_HEADER

/**
 * @brief definition of symbol object
 * symbol object includes:
 * 1. symbolName - array of chars
 * 2. baseAddress - int
 * 3. offset - int
 * 4. attributeCount - int
 * 5. attributes - double array of chars;
 */
typedef struct symbol Symbol;

/**
 * @brief Create an array of symbols, by a given size
 * 
 * @param size size of array
 * @return Symbol* 
 */
Symbol *createSymbolArray(int);

/**
 * @brief get symbol from array at given index
 * 
 * @param symbolArray pointer to symbol array
 * @param index int index
 * @return Symbol* 
 */
Symbol *getSymbolAtIndex(Symbol *, int );

/**
 * @brief Get Attributes Line by a specific index from the attributes 2d array
 * 
 * @param symbol symbol object
 * @param index int index
 * @return char* 
 */
char *getAttributesLine(Symbol *, int);

/**
 * @brief Get the Symbol Name
 * 
 * @param symbol pointer to symbol object
 * @return char* 
 */
char *getSymbolName(Symbol *);

/**
 * @brief Get the Base Address of a symbol object
 * 
 * @param symbol pointer to symbol object
 * @return int 
 */
int getBaseAddress(Symbol *);

/**
 * @brief Get the Offset of a symbol object
 * 
 * @param symbol pointer to symbol object
 * @return int 
 */
int getOffset(Symbol *);

/**
 * @brief Get the Attributes Count of a symbol object
 * 
 * @param symbol pointer to symbol object
 * @return int 
 */
int getAttributesCount(Symbol *);

/**
 * @brief increase the attribues count by 1
 * 
 * @param symbol pointer to symbol object
 */
void increaseAttributeCount(Symbol *);

/**
 * @brief Creates a Symbol in the symbol table
 * 
 * @param table symbol table
 * @param index current last index
 * @param symbolName name of symbol to create
 * @param attr attribute for symbol
 * @param base base address for symbol
 * @param offset offset from base address for symbol
 */
void createSymbol(Symbol *, int, char *, char *, int, int);


#endif