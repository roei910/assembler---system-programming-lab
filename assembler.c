/**
 * @file assembler.c
 * @author roei daniel
 * @brief the file will handle a single .as file and run the assembler program
 * will create files according to the lines read
 */
#include "assembler.h"

int main(int argc, char **argv){
    if(argc == 1)
        printf("[Error]: no input files\n");
    else
        startAssembler(argc, argv);
    return 0;
}

void startAssembler(int argc, char **argv){
    int i;
    for(i = 1; i < argc; i++){
        if(!runProgram(*(argv+i)))/*run each file name through assembler*/
            fprintf(stdout, "[Assembler Error]: found error/s in file \"%s\"\n", *(argv+i));
        else
            fprintf(stdout, "[Assembler]: file \"%s\" was successfully\n", *(argv+i));
        if(argc - i > 1)
            fprintf(stdout, NEW_LINE_FOR_PRINT);
    }
}

int runProgram(char *fileName){
    FILE *fp;
    Symbol *symbolTable = createSymbolArray(MAX_SYMBOLS);
    BinaryLine *lines = createBinaryLineArray(MAX_MACHINE_CODE_LINES);
    char *extFileName = (char *) calloc(strlen(fileName)+3, sizeof(char));
    char *amFileName = (char *) calloc(strlen(fileName)+3, sizeof(char));
    int ICF, DCF, symbolTableSize, error = 1;

    if(!preAssembler(fileName))
        return 0;
    strcpy(amFileName, fileName);
    strcat(amFileName, ".am");
    if((fp = fopen(amFileName, "r"))){ /*open .am file*/
        if(!startFirstRun(fp, symbolTable, lines, &ICF, &DCF, &symbolTableSize)){
            fprintf(stdout, "[First Run Error]: please check error/s in .am file\n");
            error = 0;
        }
        else{
            /*create extern file, .ext*/
            strcpy(extFileName, fileName);
            strcat(extFileName, ".ext");
            if(!startSecondRun(fp, extFileName, symbolTable, lines, symbolTableSize)){
                fprintf(stdout, "[Second Run Error]: please check error/s from file \"%s\"\n", amFileName);
                error = 0;
            }
            else{
                if(!buildOutPutFiles(fileName, lines, symbolTable, symbolTableSize, ICF, DCF))
                    error = 0;
            }      
        }
        /*close .am file*/
        fclose(fp);
        /*free allocated memory*/
        free(symbolTable);
        free(lines);
        free(extFileName);
        free(amFileName);
    }
    return error;
}

int preAssembler(char *fileName){
    int error = 1;
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
        fprintf(stdout, "[ERROR]: file not found: %s\n", newFileName);
        error = 0;
    }
    free(newFileName);
    return error;
}

int buildOutPutFiles(char *fileName, BinaryLine *lines, Symbol *symbolTable, int symbolTableSize, int ICF, int DCF){
    /*create object file, .ob*/
    FILE *fp;
    char *newFileName = (char *) calloc(strlen(fileName)+3, sizeof(char));
    int error = 1, i;
    strcpy(newFileName, fileName);
    strcat(newFileName, ".ob");
    
    if((fp = fopen(newFileName, "w"))){
        fprintf(fp, "\t%d\t%d\n", ICF-DCF-100, DCF);
        for(i = 0 ; i < ICF-100 ; i++){
            fprintf(fp, "%04d\t", i+100);
            printWord(fp, getBinaryAtIndex(lines, i));
        }
        fclose(fp);
    }
    else{
        fprintf(stdout, "[ERROR]: creating .ob file %s\n", newFileName);
        error = 0;
    }
    
    /*create entry file, .ent*/
    strcpy(newFileName, fileName);
    strcat(newFileName, ".ent");
    
    if((fp = fopen(newFileName, "w"))){
        printSymbolEntry(fp, symbolTable, symbolTableSize);
        fclose(fp);
    }
    else{
        fprintf(stdout, "[ERROR]: creating .ent file %s\n", newFileName);
        error = 0;
    }
    free(newFileName);
    return error;
}

void printSymbolEntry(FILE *fp, Symbol *symbolTable, int symbolTableSize){
    int i;
    Symbol *tempSymbol;
    for(i = 0 ; i < symbolTableSize ; i++){
        tempSymbol = getSymbolAtIndex(symbolTable, i);
        if(!strcmp(getAttributesLine(tempSymbol, 1), ENTRY_ATTRIBUTE))
            fprintf(fp, "%s,%d,%d\n", getSymbolName(tempSymbol), getBaseAddress(tempSymbol), getOffset(tempSymbol));
    }
}
