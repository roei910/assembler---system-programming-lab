#include "compiler.h"

void runProgram(char *fileName){
    FILE *fp;
    symbol *symbolTable = (symbol *)calloc(MAX_SYMBOLS, sizeof(symbol));
    binLine *lines = (binLine *)calloc(MAX_MACHINE_CODE_LINES, sizeof(binLine));
    char *newFileName = (char *) calloc(strlen(fileName)+3, sizeof(char));
    int ICF, DCF, symbolTableSize/*, i*/;
    preCompiler(fileName);

    strcpy(newFileName, fileName);
    strcat(newFileName, ".am");

    if((fp = fopen(newFileName, "r"))){ /*open .am file*/
        if(!startFirstRun(fp, symbolTable, lines, &ICF, &DCF, &symbolTableSize)){
            printf("Error/s while first run, please check errors\n");
            exit(0);
        }
        else{
            printf("first run was a success\n");
            printf("IC = %d, DC = %d\n", ICF, DCF);
        }
        if(!startSecondRun(fp, symbolTable, lines, symbolTableSize)){/*create start second run inside the secondRun.c*/
            printf("Error/s while second run, please check errors\n");
            exit(0);
        }
        else{
            printf("second run was a success\n");
            /*for(i = 0 ; i < symbolTableSize ; i++){
                printSymbol(symbolTable+i);
            }*/
        }
        buildOutPutFiles(lines, symbolTable, ICF, DCF);
        fclose(fp);/*close .am file*/
    }
}

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
        printf("file not found: %s\n", newFileName);
    }
}

void buildOutPutFiles(binLine *lines, symbol *symbolTable, int ICF, int DCF){
    /*create object file, .ob*/
    /*create extern file, .ext*/
    /*create entry file, .ent*/
}