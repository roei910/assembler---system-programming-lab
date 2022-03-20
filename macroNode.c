#include "macroNode.h"

struct macroNode{
    char name[MAX_MACRO_NAME];
    char macroContent[MAX_MACRO_LENGTH][MAX_LINE]; 
    int lines;
    struct macroNode *next;
};

MacroNode *newMacroNodeList(){
    return (MacroNode *)malloc(sizeof(MacroNode));
}

char *getMacroNodeName(MacroNode *macroNode){
    return macroNode->name;
}

char *getMacroNodeContentAtIndex(MacroNode *macroNode, int index){
    return macroNode->macroContent[index];
}

void createNewNode(FILE *file, char *macroName, MacroNode *node){
    int counter = 0;
    char lineBuffer[MAX_LINE];
    sscanf(macroName+5, " %s", getMacroNodeName(node));
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

MacroNode *getNext(MacroNode *macroNode){
    return macroNode->next;
}

void setNextNode(MacroNode *macroNode, MacroNode *newNode){
    macroNode->next = newNode;
    macroNode = macroNode->next;
    macroNode->next = NULL;
}

int getMacroLines(MacroNode *macroNode){
    return macroNode->lines;
}