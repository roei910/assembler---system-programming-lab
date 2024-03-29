#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include "constants.h"
#include "decodeLine.h"

/**
 * @file decodeLine.c
 * @author roei daniel
 * @brief the file will help with line decoding for first and second run
 */

int lineDecode(int line, char *inputLine, char *command, char *src, char *dest, int *srcAddressing, int *destAddressing, int *numberOfOpperands){
    int valid, linesAdded = 0;
    *numberOfOpperands = decodeInstructionLine(inputLine, command, src, dest, line);
    switch (*numberOfOpperands)
    {
        case 0:
            return ++linesAdded;
        case 1:
            valid = checkValidCommandOneOpperand(command, dest, destAddressing);
            if(valid){
                if(*destAddressing == 0)
                    linesAdded++;
                if((*destAddressing == 1) || (*destAddressing == 2))
                    linesAdded+=2;
                return linesAdded+=2;                
            }
            else
                fprintf(stdout, "[ERROR]: line:%d, opperand does not fit the command: \"%s, %s\"\n", line, command, dest);
            break;
        case 2:
            valid = checkValidCommandTwoOpperands(command, src, srcAddressing, dest, destAddressing);
            if(!valid){
                if(*srcAddressing == 0)
                    linesAdded++;
                else if((*srcAddressing == 1) || (*srcAddressing == 2))
                    linesAdded+=2;
                if(*destAddressing == 0)
                    linesAdded++;
                else if((*destAddressing == 1) || (*destAddressing == 2))
                    linesAdded+=2;
                return linesAdded+=2;     
            }
            else{
                if(valid == 3)
                    fprintf(stdout, "[ERROR]: line:%d, source and destination opperands do not fit the command: \"%s\"\n", line, command);
                else if(valid == 1)
                    fprintf(stdout, "[ERROR]: line:%d, source opperand does not fit the command: \"%s, %s\"\n", line, command, src);
                else
                    fprintf(stdout, "[ERROR]: line:%d, destination opperand does not fit the command: \"%s, %s\"\n", line, command, dest);
            }
            break;
        case -1:
            break;
    }
    return 0;
}

int decodeInstructionLine(char *inputLine, char *command, char *src, char *dest, int line){
    char opperands[2*MAX_OPPERAND_LENGTH], *ptr, inputLineCpy[MAX_LINE], tempC;
    int countOpp, numOfopp, flag = 0;
    strcpy(inputLineCpy, inputLine);
    if(!(countOpp = sscanf(inputLine, "%s %s", command, opperands)))
        return 0;/*couldnt find a command*/
    if(countOpp == 2){/*there is at least 1 opperand*/
        ptr = strtok(strstr(inputLineCpy, command) + strlen(command), COMMA_SYMBOL);
        if((flag = sscanf(ptr, "%s %c", src, &tempC)) == -1){
            fprintf(stdout, "[ERROR]: line:%d, illegal comma\n", line);
            return -1;
        }
        if(flag == 2){
            fprintf(stdout, "[ERROR]: line:%d, missing comma / illegal opperand\n", line);
            return -1;
        }
        if(ptr != NULL){
            ptr = strtok(NULL, COMMA_SYMBOL);
            if(ptr == NULL){
                strcpy(dest, src);
                countOpp = 1;
            }
            else{
                if((flag = sscanf(ptr, " %s %c", dest, &tempC)) == -1){
                    fprintf(stdout, "[ERROR]: line:%d, missing comma\n", line);
                    return -1;
                }
                if(flag == 2){
                    fprintf(stdout, "[ERROR]: line:%d, missing comma / illegal opperand\n", line);
                    return -1;
                }
                ptr = strtok(NULL, COMMA_SYMBOL);
                if(ptr != NULL){
                    countOpp++;
                }
            }
        }
    }
    else
        countOpp = 0;
    numOfopp = getNumberOfOpperands(command);/*get number of opperands by command name*/
    if(numOfopp == -1){
        if(strstr(command, COMMA_SYMBOL))
            fprintf(stdout, "[ERROR]: line:%d, illegal comma in command name: \"%s\"\n", line, command);
        else
            fprintf(stdout, "[ERROR]: line:%d, illegal command name: \"%s\"\n", line, command);
        return -1;
    }
    if(numOfopp == countOpp)
        return countOpp;
    fprintf(stdout, "[ERROR]: line:%d, number of opperands is incorrect: \"%s\", number of opperands should be %d\n", line, command, numOfopp);
    return -1;
}

int checkOpperandType(char *opperand){
    char *ptr;
    if((ptr = strstr(opperand, HASHTAG_SYMBOL))){
        if(isNumber(ptr+1) == 1)
            return 0;
    }
    else if(strstr(opperand, LEFT_SQUARE_BRACKET)){
        if(isIndexOpperand(opperand))
            return 2;
    }
    else if(isRegister(opperand))
        return 3;
    else if(checkValidSymbol(opperand))
        return 1;
    return -1;
}

int isNumber(char *opperand){
    int num;
    return sscanf(opperand, "%d", &num);
}

int isRegister(char *opperand){
    char c;
    int num;
    sscanf(opperand, "%c%d", &c, &num);
    if(c == 'r')
        return 1;
    return 0;
}

int isIndexOpperand(char *opperand){
    char *ptr;
    char reg[MAX_NAME], label[MAX_LABEL_LENGTH], cpyOpperand[MAX_OPPERAND_LENGTH];
    if((ptr = strstr(opperand, LEFT_SQUARE_BRACKET))){
        sscanf(ptr+1, "%s", reg);/*get register from opperand*/
        ptr = strstr(reg, RIGHT_SQUARE_BRACKET);
        if(ptr != NULL)
            *ptr = '\0';/*replace ] with 0*/
        
        strcpy(cpyOpperand, opperand);
        ptr = strtok(cpyOpperand, LEFT_SQUARE_BRACKET);
        sscanf(ptr, "%s", label);/*get label from opperand*/
        if((isRegister(reg) == 1) && (checkValidSymbol(label) == 1))
            return 1;
    }
    return 0;
}

int getNumberOfOpperands(char *command){
    if((!strcmp(command, COMMAND_MOV)) || (!strcmp(command, COMMAND_CMP)) ||
        (!strcmp(command, COMMAND_ADD)) || (!strcmp(command, COMMAND_SUB)) ||
        (!strcmp(command, COMMAND_LEA))){
        return 2;
    }
    else if((!strcmp(command, COMMAND_CLR)) || (!strcmp(command, COMMAND_NOT)) ||
        (!strcmp(command, COMMAND_INC)) || !strcmp(command, COMMAND_DEC) ||
        (!strcmp(command, COMMAND_JMP)) || (!strcmp(command, COMMAND_BNE)) ||
        (!strcmp(command, COMMAND_JSR)) || (!strcmp(command, COMMAND_RED)) ||
        (!strcmp(command, COMMAND_PRN))){
        return 1;
    }
    else if((!strcmp(command, COMMAND_RTS)) || (!strcmp(command, COMMAND_STOP))){
        return 0;
    }
    return -1;/*invalid command*/
}

int checkValidCommandTwoOpperands(char *command, char *src, int *srcAddressing, char *dest, int *destAddressing){
    int valid = 0;
    *destAddressing = checkOpperandType(dest);
    *srcAddressing = checkOpperandType(src);
    if((*srcAddressing == 0) || (*srcAddressing == 3)){
        if(!strcmp(command, COMMAND_LEA))
            valid = 1;
    }
    if(*destAddressing == 0){
        if(strcmp(command, COMMAND_CMP))
            valid += 2;
    }
    return valid;
}

int checkValidCommandOneOpperand(char *command, char *dest, int *destAddressing){
    *destAddressing = checkOpperandType(dest);
    if(*destAddressing == 0){
        if(!strcmp(command, COMMAND_PRN))
            return 1;
        return 0;
    }
    else if(*destAddressing == 3){
        if(((!strcmp(command, COMMAND_JMP))) || (!strcmp(command, COMMAND_BNE)) ||
            (!strcmp(command, COMMAND_JSR)))
            return 0;
        return 1;
    }
    else if(*destAddressing == -1)
        return 0;
    return 1;
}

int getFunct(char *command){
    if((!strcmp(command, COMMAND_ADD)) || (!strcmp(command, COMMAND_CLR)) ||
        (!strcmp(command, COMMAND_JMP)))
        return 10;
    if((!strcmp(command, COMMAND_SUB)) || (!strcmp(command, COMMAND_NOT)) ||
        (!strcmp(command, COMMAND_BNE)))
        return 11;
    if((!strcmp(command, COMMAND_INC)) || (!strcmp(command, COMMAND_JSR)))
        return 12;
    if(!strcmp(command, COMMAND_DEC))
        return 13;
    return 0;
}

int getOpcode(char *command){
    if(!strcmp(command, COMMAND_MOV))
        return 0;
    if(!strcmp(command, COMMAND_CMP))
        return 1;
    if((!strcmp(command, COMMAND_ADD)) || (!strcmp(command, COMMAND_SUB)))
        return 2;
    if(!strcmp(command, COMMAND_LEA))
        return 4;
    if((!strcmp(command, COMMAND_CLR)) || (!strcmp(command, COMMAND_NOT)) ||
        (!strcmp(command, COMMAND_INC)) || (!strcmp(command, COMMAND_DEC)))
        return 5;
    if((!strcmp(command, COMMAND_JMP)) || (!strcmp(command, COMMAND_BNE)) ||
        (!strcmp(command, COMMAND_JSR)))
        return 9;
    if(!strcmp(command, COMMAND_RED))
        return 12;
    if(!strcmp(command, COMMAND_PRN))
        return 13;
    if(!strcmp(command, COMMAND_RTS))
        return 14;
    if(!strcmp(command, COMMAND_STOP))
        return 15;
    return -1;
}

int buildMachineCodeLines(int linesCounter, BinaryLine *lines, char *command, int arguments, ...){
    int dest, destAddressing, src, srcAddressing, linesWritten = 0;
    char *destPtr, *srcPtr;
    va_list valist;
    va_start(valist, arguments);
    switch (arguments)
    {
        case 0:
            createBinaryLine(getBinaryAtIndex(lines, (linesWritten++)), 2, MACHINE_CODE_A, (unsigned int)pow(2, getOpcode(command)));
            return linesWritten;
        case 2:/*arguments: DEST REG, DEST ADDRESSING*/
            destPtr = va_arg(valist, char *);
            destAddressing = va_arg(valist, int);
            if(destAddressing == 0)
                dest = getNumberFromOpperand(destPtr);
            else if(destAddressing != 1)
                dest = getRegFromOpperand(destPtr);
            else
                dest = 0;
            if(dest == -1){
                fprintf(stdout, "[ERROR]: line:%d, register is invalid, command: \"%s\"\n", linesCounter, command);
                return 0;
            }
            createBinaryLine(getBinaryAtIndex(lines, (linesWritten++)), 2, MACHINE_CODE_A, (unsigned int)pow(2, getOpcode(command)));
            createBinaryLine(getBinaryAtIndex(lines, (linesWritten++)), 4, MACHINE_CODE_A, getFunct(command), dest, destAddressing);
            if(destAddressing == 0)
                createBinaryLine(getBinaryAtIndex(lines, (linesWritten++)), 2, MACHINE_CODE_A, dest);
            if((destAddressing == 1) || (destAddressing == 2)){
                createBinaryLine(getBinaryAtIndex(lines, (linesWritten++)), 2, 3, 0);
                createBinaryLine(getBinaryAtIndex(lines, (linesWritten++)), 2, 3, 0);
            }
            return linesWritten;
        case 4:
            srcPtr = va_arg(valist, char *);
            srcAddressing = va_arg(valist, int);
            if(srcAddressing == 0)
                src = getNumberFromOpperand(srcPtr);
            else if(srcAddressing != 1)
                src = getRegFromOpperand(srcPtr);
            else
                src = 0;
            if(src == -1){
                fprintf(stdout, "[ERROR]: line:%d, register is invalid, command: \"%s\"\n", linesCounter, command);
                return 0;
            }
            destPtr = va_arg(valist, char *);
            destAddressing = va_arg(valist, int);
            if(destAddressing == 0)
                dest = getNumberFromOpperand(destPtr);
            else if(destAddressing != 1)
                dest = getRegFromOpperand(destPtr);
            else
                dest = 0;
            if(dest == -1){
                fprintf(stdout, "[ERROR]: line:%d, register is invalid, command: \"%s\"\n", linesCounter, command);
                return 0;
            }
            createBinaryLine(getBinaryAtIndex(lines, (linesWritten++)), 2, MACHINE_CODE_A, (int)pow(2, getOpcode(command)));
            createBinaryLine(getBinaryAtIndex(lines, (linesWritten++)), 6, MACHINE_CODE_A, getFunct(command), (srcAddressing == 0)? 0 : src, srcAddressing, (destAddressing == 0)? 0 : dest, destAddressing);          
            if(srcAddressing == 0)
                createBinaryLine(getBinaryAtIndex(lines, (linesWritten++)), 2, MACHINE_CODE_A, src);
            if((srcAddressing == 1) || (srcAddressing == 2)){
                createBinaryLine(getBinaryAtIndex(lines, (linesWritten++)), 2, 3, 0);
                createBinaryLine(getBinaryAtIndex(lines, (linesWritten++)), 2, 3, 0);
            }
            if(destAddressing == 0)
                createBinaryLine(getBinaryAtIndex(lines, (linesWritten++)), 2, MACHINE_CODE_A, dest);
            if((destAddressing == 1) || (destAddressing == 2)){
                createBinaryLine(getBinaryAtIndex(lines, (linesWritten++)), 2, 3, 0);
                createBinaryLine(getBinaryAtIndex(lines, (linesWritten++)), 2, 3, 0);
            }
            return linesWritten;
    }
    return 0;
}

int getNumberFromOpperand(char *opperand){
    int number=0;
    if(sscanf(opperand+1, "%d", &number) == 0) 
        return -1;
    return number;
}

int getRegFromOpperand(char *opperand){
    char *ptr;
    int reg;
    ptr = strstr(opperand, REGISTER_R_SYMBOL);
    if(ptr == NULL)
        return -1;
    reg = getNumberFromOpperand(ptr);
    if(reg >= 0 && reg <= 15)
        return reg;
    return -1;
}

int checkValidSymbol(char *symbol){
    int countAlpha = 0;
    if(getRegFromOpperand(symbol) != -1)
        return 0;
    if(getOpcode(symbol) != -1)
        return 0;
    while(*symbol != '\0'){
        if(!isalnum(*symbol))
            return 0;
        if(isalpha(*symbol))
            countAlpha++;
        symbol++;
    }
    if(!countAlpha)
        return 0;
    return 1;
}

int isSymbolDecleration(char *inputLine){
    char *cPointer;
    if((cPointer = strstr(inputLine, LABEL_SYMBOL)) != NULL)
        return 1;
    return 0;
}

int isDataDecleration(char *inputLine){
    if(checkDecleration(inputLine, DATA_DECLERATION))
        return 1;
    else if(checkDecleration(inputLine, STRING_DECLERATION))
        return 1;
    return 0;
}

int isExternDecleration(char *inputLine){
    if(checkDecleration(inputLine, EXTERN_DECLERATION))
        return 1;
    return 0;
}

int skipSymbol(char *inputLine){
    char *ptr;
    if((ptr = strstr(inputLine, LABEL_SYMBOL))){
        ptr += strlen(LABEL_SYMBOL);
        while((*ptr) == ' '){
            if(isalnum(*ptr))
                break;
            ptr++;
        }
        strcpy(inputLine, ptr);
        return 1;
    }
    return 0;
}

int checkDecleration(char *inputLine, char *decleration){
    if(strstr(inputLine, decleration) != NULL)
        return 1;
    return 0;
}

int findSymbolInTable(Symbol *table, int tableSize, char *symbolName){
    int i;
    Symbol *symbolPtr;
    for(i = 0 ; i < tableSize ; i++){
        symbolPtr = getSymbolAtIndex(table, i);
        if(!strcmp(getSymbolName(symbolPtr), symbolName))
            return i;
    }
    return -1;
}

int isEmptyLine(char *inputLine){
    char tempString[MAX_LINE];
    return (sscanf(inputLine, "%s\n", tempString) == -1);
}

int isCommentLine(char *inputLine){
    return strstr(inputLine, COMMENT_LINE_STRING) != NULL;
}

int isEntryDecleration(char *inputLine){
    if(checkDecleration(inputLine, ENTRY_DECLERATION))
        return 1;
    return 0;
}
