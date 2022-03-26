#if !defined(SECOND_RUN_HEADER)
#define SECOND_RUN_HEADER

/**
 * @brief the function will start second run, will return FALSE if found errors
 * 
 * @param fp FILE pointer to the assembly file after macro
 * @param extFileName name of .ext file 
 * @param symbolTable symbol table
 * @param lines binary lines for the file read
 * @param tableSize size of symbol table
 * @return int 
 */
int startSecondRun(FILE *, char *, Symbol *, BinaryLine *, int);

/**
 * @brief adds an attribute to the symbol from symbol table array
 * 
 * @param linesCounter number of line for errors
 * @param table symbol table
 * @param tableSize size of symbol table
 * @param symbolName symbol to add an attribute
 * @param attr attribute to add
 * @return int True for successfull, False for fail in adding attribute
 */
int addAttribute(int, Symbol *, int, char *, char *);

/**
 * @brief extracts the symbol from the entry line to tempsymbol parameter
 * 
 * @param inputLine input line from file
 * @param tempSymbol destination for symbol
 * @return int 
 */
int extractSymbolFromEntryLine(char *, char *);

/**
 * @brief Get the Symbol From an Opperand to the tempsymbol
 * 
 * @param opperand the opperand to be read
 * @param tempSymbol destination for symbol
 */
void getSymbolFromOpperand(char *, char *);

/**
 * @brief build the symbol lines to the binline array
 * 
 * @param linesCounter current line to be read from file after macro
 * @param fp FILE pointer to .ext file
 * @param lines binary lines
 * @param symbolTable symbol table
 * @param tableSize size of symbol table
 * @param opperand the opperand in the symbol line
 * @param IC instruction count for the assembly file
 * @return int 
 */
int buildSymbolLines(int, FILE **, char *, BinaryLine *, Symbol *, int , char *, int);

/**
 * @brief prints the use of a symbol to the ext file
 * 
 * @param fp FILE pointer to the ext file
 * @param extSymbol the symbol to be printed to file
 * @param baseAddress base address for symbol use in assembly file
 */
void printSymbolExternal(FILE *, Symbol *, int);

#endif