#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "constants.h"
#include "preAssembler.h"

/**
 * @file preAssembler.c
 * @author roei daniel
 * @brief preAssembler replace all macros in file, create a new file with .am extension 
 * @copyright Copyright (c) 2022
 */

int replaceAllMacro(FILE *fp, char *fileName){
    char lineBuffer[MAX_LINE], *cp, macroName[MAX_MACRO_NAME];
    int counter;
    MacroNode *replaceMacro, *macro = NULL;
    MacroNode *temp = newMacroNodeList();
    FILE *secondfp;
    secondfp = fopen(fileName, "w");
    writeNewFileName(secondfp, fileName);
    while(fgets(lineBuffer, MAX_LINE, fp) != NULL){/*read each line*/
        strcpy(macroName, "");
        if((cp = isMacro(lineBuffer)) != NULL){
            createNewNode(fp, cp, temp);
            insertMacro(&macro, temp);
        }
        else{
            sscanf(lineBuffer, " %s", macroName);
            if((replaceMacro = findMacro(macroName, macro)) != NULL){
                fprintMacro(replaceMacro, secondfp);
            }
            else
                fprintf(secondfp, "%s", lineBuffer);
        }
        counter++;
    }
    fclose(secondfp);
    free(temp);
    return 0;
}

MacroNode *findMacro(char *macroName, MacroNode *macro){
    MacroNode *temp = macro;
    while(temp != NULL){
        if(!strcmp( getMacroNodeName(temp), macroName))
            return temp;
        temp = getNext(temp);
    }
    return NULL;
} 

char *isMacro(char *inputLine){
    return strstr(inputLine, "macro");
}

void insertMacro(MacroNode **macro, MacroNode *newNode){
    MacroNode *temp = *macro;
    if(*macro == NULL)
        *macro = newNode;
    else{
        while( getNext(temp) != NULL){
            temp = getNext(temp);
        }
        setNextNode(temp, newNode);
    }
    
}

void fprintMacro(MacroNode *macro, FILE *fp){
    int i;
    for(i = 0 ; i < getMacroLines(macro) ; i++)
        fprintf(fp, "%s",getMacroNodeContentAtIndex(macro, i));
}

void writeNewFileName(FILE *fp, char *fileName){
    char *tempFileName;
    tempFileName = strstr(fileName, "/");
    if(tempFileName != NULL)
        fprintf(fp, "; file %s\n", tempFileName+1);
    else
        fprintf(fp, "; file %s\n", fileName);
}