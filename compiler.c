#include "compiler.h"

void runProgram(char *fileName){
    FILE *fp;
    char *newFileName = (char *) calloc(strlen(fileName)+3, sizeof(char));
    preCompiler(fileName);

    strcpy(newFileName, fileName);
    strcat(newFileName, ".am");

    if((fp = fopen(newFileName, "r"))){ /*open .am file*/
        if(!startFirstRun(fp)){
            printf("Error/s while first run, please check errors\n");
            exit(0);
        }
        else{
            printf("first run was a success\n");
        }
        secondRun(fp);/*create start second run inside the secondRun.c*/
        buildOutPutFiles(fp);
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

void secondRun(FILE *fp){
    
}

void buildOutPutFiles(FILE *fp){
    /*create object file, .ob*/
    /*create extern file, .ext*/
    /*create entry file, .ent*/
}