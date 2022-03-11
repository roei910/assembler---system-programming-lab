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
    macroNode *replaceMacro, *macro = NULL;
    macroNode *temp = (macroNode *)malloc(sizeof(macroNode));
    FILE *secondfp;
    secondfp = fopen(fileName, "w");
    writeNewFileName(secondfp, fileName);
    fgets(lineBuffer, MAX_LINE, fp);
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

void createNewNode(FILE *file, char *macroName, macroNode *node){
    int counter = 0;
    char lineBuffer[MAX_LINE];
    sscanf(macroName+5, " %s", node->name);
    while(fgets(lineBuffer, MAX_LINE, file) != NULL){
        if(strstr(lineBuffer, "endm") != NULL)
            break;
        else
            strcpy(node->macroContent[counter], lineBuffer);
        counter++;
    }
    node->lines = counter;
    node->next = NULL;
}

macroNode *findMacro(char *macroName, macroNode *macro){
    macroNode *temp = macro;
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

void insertMacro(macroNode **macro, macroNode *newNode){
    macroNode *temp = *macro;
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

void fprintMacro(macroNode *macro, FILE *fp){
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