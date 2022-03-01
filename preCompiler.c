/**
 * @file preCompiler.c
 * @author roei daniel
 * @brief preCompiler replace all macros in file, create a new file with .am extension 
 * @copyright Copyright (c) 2022
 */
#include "preCompiler.h"

int replaceAllMacro(FILE *fp, char *fileName){
    char lineBuffer[MAX_LINE], *cp, macroName[MAX_MACRO_NAME];
    int counter;
    MacroNode *temp, *replaceMacro, *macro = NULL;
    FILE *secondfp;
    secondfp = fopen(fileName, "w");
    writeNewFileName(secondfp, fileName);
    fgets(lineBuffer, MAX_LINE, fp);
    while(fgets(lineBuffer, MAX_LINE, fp) != NULL){/*read each line*/
        if((cp = isMacro(lineBuffer)) != NULL){
            temp = createNewNode(cp, &fp);
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
    return 0;
}

MacroNode *createNewNode(char *macroName, FILE **file){
    int counter = 1;
    MacroNode *temp = (MacroNode *)malloc(sizeof(MacroNode));
    char lineBuffer[MAX_LINE];
    FILE *fp = *file;
    sscanf(macroName+5, " %s", temp->name);
    temp->startingLine = counter+1;
    while(fgets(lineBuffer, MAX_LINE, fp) != NULL){
        counter++;
        if(strstr(lineBuffer, "endm") != NULL)
            break;
        else
            strcpy(temp->macroContent[counter-2], lineBuffer);
    }
    temp->lines = counter - temp->startingLine;
    temp->next = NULL;
    return temp;
}

MacroNode *findMacro(char *macroName, MacroNode *macro){
    MacroNode *temp = macro;
    while(temp != NULL){
        if(!strcmp(temp->name, macroName))
            return temp;
        temp = temp->next;
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
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = newNode;
        temp = temp->next;
        temp->next = NULL;
    }
    
}

void fprintMacro(MacroNode *macro, FILE *fp){
    int i;
    for(i = 0 ; i < macro->lines ; i++)
        fprintf(fp, "%s", macro->macroContent[i]);
}

void writeNewFileName(FILE *fp, char *fileName){
    char *tempFileName;
    tempFileName = strstr(fileName, "/");
    if(tempFileName != NULL)
        fprintf(fp, "; file %s\n", tempFileName+1);
    else    
        fprintf(fp, "; file %s\n", fileName);
}