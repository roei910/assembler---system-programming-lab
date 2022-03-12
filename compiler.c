/**
 * @file compiler.c
 * @author roei daniel
 * @brief the file will handle a single .as file and run the compiler program
 * will create files according to the lines read
 */
#include "compiler.h"

/**
 * @brief run compiler for a file name
 * 
 * @param fileName the file to be compiled
 */
void runProgram(char *fileName){
    FILE *fp, *extFp;
    symbol *symbolTable = (symbol *)calloc(MAX_SYMBOLS, sizeof(symbol));
    binLine *lines = (binLine *)calloc(MAX_MACHINE_CODE_LINES, sizeof(binLine));
    char *newFileName = (char *) calloc(strlen(fileName)+3, sizeof(char));
    int ICF, DCF, symbolTableSize;

    preCompiler(fileName);
    strcpy(newFileName, fileName);
    strcat(newFileName, ".am");
    
    if((fp = fopen(newFileName, "r"))){ /*open .am file*/
        if(!startFirstRun(fp, symbolTable, lines, &ICF, &DCF, &symbolTableSize)){
            fprintf(stderr, "[First Run Error]: please check error/s\n");
            exit(0);
        }
        
        /*create extern file, .ext*/
        strcpy(newFileName, fileName);
        strcat(newFileName, ".ext");
        
        if(!(extFp = fopen(newFileName, "w")))
            fprintf(stderr, "[ERROR]: creating .ext file %s\n", newFileName);

        if(!startSecondRun(fp, extFp, symbolTable, lines, symbolTableSize)){/*create start second run inside the secondRun.c*/
            fprintf(stderr, "[Second Run Error]: please check error/s\n");
            exit(0);
        }
        
        if(extFp)
            fclose(extFp);
        buildOutPutFiles(fileName, lines, symbolTable, symbolTableSize, ICF, DCF);
        fclose(fp);/*close .am file*/
        /*free allocated memory*/
        free(symbolTable);
        free(lines);
        free(newFileName);
    }
}

/**
 * @brief run the file through precompiler program
 * 
 * @param fileName the file name to be run precompiler
 */
void preCompiler(char *fileName){
    FILE *fp;
    char *newFileName = (char *) calloc(strlen(fileName)+3, sizeof(char));
    strcpy(newFileName, fileName);
    strcat(newFileName, ".as");
    
    if((fp = fopen(newFileName, "r"))){ /*open .as file*/
        newFileName[strlen(newFileName)-1] = 'm';
        replaceAllMacro(fp, newFileName);/*create .am file*/
        fclose(fp);/*close .as file*/
    }
    else{
        fprintf(stderr, "[ERROR]: file not found: %s\n", newFileName);
    }
    free(newFileName);
}

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
void buildOutPutFiles(char *fileName, binLine *lines, symbol *symbolTable, int symbolTableSize, int ICF, int DCF){
    /*create object file, .ob*/
    FILE *fp;
    char *newFileName = (char *) calloc(strlen(fileName)+3, sizeof(char));
    int i;
    strcpy(newFileName, fileName);
    strcat(newFileName, ".ob");
    
    if((fp = fopen(newFileName, "w"))){
        fprintf(fp, "\t%d\t%d\n", ICF-DCF-100, DCF);
        for(i = 0 ; i < ICF-100 ; i++){
            fprintf(fp, "%04d\t", i+100);
            printWord(fp, *(lines+i));
        }
        fclose(fp);
    }
    else{
        fprintf(stderr, "[ERROR]: creating .ob file %s\n", newFileName);
    }
    
    /*create entry file, .ent*/
    strcpy(newFileName, fileName);
    strcat(newFileName, ".ent");
    
    if((fp = fopen(newFileName, "w"))){
        printSymbolEntry(fp, symbolTable, symbolTableSize);
        fclose(fp);
    }
    else{
        fprintf(stderr, "[ERROR]: creating .ent file %s\n", newFileName);
    }
    free(newFileName);
}

/**
 * @brief print an entry symbols according to the instructions
 * 
 * @param fp file to be written to
 * @param symbolTable symbols read from the file
 * @param symbolTableSize size of symbol table array
 */
void printSymbolEntry(FILE *fp, symbol *symbolTable, int symbolTableSize){
    int i;
    for(i = 0 ; i < symbolTableSize ; i++){
        if(!strcmp((symbolTable+i)->attributes[1], ENTRY_ATTRIBUTE))
            fprintf(fp, "%s,%d,%d\n", (symbolTable+i)->symbol, (symbolTable+i)->baseAddress, (symbolTable+i)->offset);
    }
}
