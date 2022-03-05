#include "decodeLine.h"

int lineDecode(char *inputLine, binLine *lines, symbol *symbolTable, int symbolCount){
    int valid, srcAddressing, destAddresing, linesAdded;
    char command[MAX_COMMAND_NAME_LENGTH], src[MAX_OPPERAND_LENGTH], dest[MAX_OPPERAND_LENGTH];
    valid = decodeInstructionLine(inputLine, command, src, dest);
    switch (valid)
    {
        case 0:
            /*printf("valid command - no opperands\n");*/
            buildMachineCodeLines(lines, symbolCount, symbolTable, command, 0);
            return 1;
        case 1:
            /*printf("valid command - 1 opperand, ");*/
            valid = checkValidCommandOneOpperand(command, dest, &destAddresing);
            if(valid){
                /*printf("valid opperand, dest addressing = %d, dest = %s\n", destAddresing, dest);*/
                linesAdded = buildMachineCodeLines(lines, symbolCount, symbolTable, command, 2, dest, destAddresing);
                return linesAdded;                
            }
            else
                fprintf(stderr, "[ERROR]: opperand does not fit the command: \"%s, %s\"\n", command, dest);
            break;
        case 2:
            /*printf("valid command - 2 opperands, ");*/
            valid = checkValidCommandTwoOpperands(command, src, &srcAddressing, dest, &destAddresing);
            if(!valid){
                /*printf("valid opperands\n");*/
                linesAdded = buildMachineCodeLines(lines, symbolCount, symbolTable, command, 4, src, srcAddressing, dest, destAddresing);
                return linesAdded;
            }
            else{
                if(valid == 1)
                    fprintf(stderr, "[ERROR]: source opperand does not fit the command: \"%s, %s\"\n", command, src);
                else
                    fprintf(stderr, "[ERROR]: destination opperand does not fit the command: \"%s, %s\"\n", command, dest);
            }
            break;
        case -1:
            fprintf(stderr, "[ERROR]: number of opperands is incorrect: \"%s\", number of opperands should be %d\n", command, numberOfOpperands(command));
            break;
        case -2:
            fprintf(stderr, "[ERROR]: illegal command name: \"%s\"\n", command);
            break;
    }
    return 0;
}

/**
 * @brief decode an instruction line to binary code
 * 
 */
int decodeInstructionLine(char *inputLine, char *command, char *src, char *dest){
    char opperands[2*MAX_OPPERAND_LENGTH], *ptr, inputLineCpy[MAX_LINE];
    int countOpp, numOfopp;
    strcpy(inputLineCpy, inputLine);
    if(!(countOpp = sscanf(inputLine, "%s %s", command, opperands)))
        return 0;/*couldnt find a command*/
    if(countOpp == 2){/*there is at least 1 opperand*/
        ptr = strtok(strstr(inputLineCpy, command) + strlen(command), COMMA_SYMBOL);
        sscanf(ptr, "%s", src);
        if(ptr != NULL){
            ptr = strtok(NULL, COMMA_SYMBOL);
            if(ptr == NULL){
                strcpy(dest, src);
                countOpp = 1;
            }
            else
                sscanf(ptr, "%s", dest);
        }
    }
    else
        countOpp = 0;
    numOfopp = numberOfOpperands(command);/*get number of opperands by command name*/
    if(numOfopp == -1)
        return -2;/*illegal command name*/
    if(numOfopp == countOpp)
        return countOpp;
    return -1;/*number of opps is incorrect*/
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
    if((c == 'r') && (num >= 0) && (num <= 15))
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

int numberOfOpperands(char *operation){
    if((!strcmp(operation, COMMAND_MOV)) || (!strcmp(operation, COMMAND_CMP)) ||
        (!strcmp(operation, COMMAND_ADD)) || (!strcmp(operation, COMMAND_SUB)) ||
        (!strcmp(operation, COMMAND_LEA))){
        return 2;
    }
    else if((!strcmp(operation, COMMAND_CLR)) || (!strcmp(operation, COMMAND_NOT)) ||
        (!strcmp(operation, COMMAND_INC)) || !strcmp(operation, COMMAND_DEC) ||
        (!strcmp(operation, COMMAND_JMP)) || (!strcmp(operation, COMMAND_BNE)) ||
        (!strcmp(operation, COMMAND_JSR)) || (!strcmp(operation, COMMAND_RED)) ||
        (!strcmp(operation, COMMAND_PRN))){
        return 1;
    }
    else if((!strcmp(operation, COMMAND_RTS)) || (!strcmp(operation, COMMAND_STOP))){
        return 0;
    }
    return -1;/*invalid command*/
}

/**
 * @brief first group has 2 opperands
 */
int checkValidCommandTwoOpperands(char *command, char *src, int *srcAddressing, char *dest, int *destAddressing){
    *destAddressing = checkOpperandType(dest);
    *srcAddressing = checkOpperandType(src);
    if((*srcAddressing == 0) || (*srcAddressing == 3)){
        if(!strcmp(command, COMMAND_LEA))
            return 1;
    }
    if(*destAddressing == 0){
        if(strcmp(command, COMMAND_CMP))
            return 2;
    }
    return 0;
}

/**
 * @brief second group has 1 opperand
 */
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

int buildMachineCodeLines(binLine *lines, int symbolCount, symbol *symbolTable, char *command, int arguments, ...){
    int dest, destAddressing, src, srcAddressing, linesWritten = 0;
    char *destPtr, *srcPtr;
    va_list valist;
    va_start(valist, arguments);
    switch (arguments)
    {
        case 0:
            createBinaryLine(lines+(linesWritten++), 2, MACHINE_CODE_A, (unsigned int)pow(2, getOpcode(command)));
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
            createBinaryLine(lines+(linesWritten++), 2, MACHINE_CODE_A, (unsigned int)pow(2, getOpcode(command)));
            createBinaryLine(lines+(linesWritten++), 4, MACHINE_CODE_A, getFunct(command), dest, destAddressing);
            if(destAddressing == 0)
                createBinaryLine(lines+(linesWritten++), 2, MACHINE_CODE_A, dest);
            if((destAddressing == 1) || (destAddressing == 2)){
                createBinaryLine(lines+(linesWritten++), 2, 3, 0);
                createBinaryLine(lines+(linesWritten++), 2, 3, 0);
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
            destPtr = va_arg(valist, char *);
            destAddressing = va_arg(valist, int);
            if(destAddressing == 0)
                dest = getNumberFromOpperand(destPtr);
            else if(destAddressing != 1)
                dest = getRegFromOpperand(destPtr);
            else
                dest = 0;
            createBinaryLine(lines+(linesWritten++), 2, MACHINE_CODE_A, (int)pow(2, getOpcode(command)));
            createBinaryLine(lines+(linesWritten++), 6, MACHINE_CODE_A, getFunct(command), (srcAddressing == 0)? 0 : src, srcAddressing, (destAddressing == 0)? 0 : dest, destAddressing);          
            if(srcAddressing == 0)
                createBinaryLine(lines+(linesWritten++), 2, MACHINE_CODE_A, src);
            if((srcAddressing == 1) || (srcAddressing == 2)){
                createBinaryLine(lines+(linesWritten++), 2, 3, 0);
                createBinaryLine(lines+(linesWritten++), 2, 3, 0);
            }
            if(destAddressing == 0)
                createBinaryLine(lines+(linesWritten++), 2, MACHINE_CODE_A, dest);
            if((destAddressing == 1) || (destAddressing == 2)){
                createBinaryLine(lines+(linesWritten++), 2, 3, 0);
                createBinaryLine(lines+(linesWritten++), 2, 3, 0);
            }
            return linesWritten;
    }
    return 0;
}

int getNumberFromOpperand(char *opperand){
    int number=0;
    sscanf(opperand+1, "%d", &number);
    return number;
}

int getRegFromOpperand(char *opperand){
    char *ptr;
    ptr = strstr(opperand, REGISTER_R_SYMBOL);
    return getNumberFromOpperand(ptr);
}

int checkValidSymbol(char *symbol){
    int countAlpha = 0;
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

int checkSymbolType(int tableSize, symbol *symbolTable, char *symbolName){
    int index, i;
    index = findSymbolInTable(symbolTable, tableSize, symbolName);
    for (i = 0; i < (symbolTable+index)->attributeCount; i++){
        if(isExtern((symbolTable+index)->attributes[i]))
            return 1;
        else if(isEntry((symbolTable+index)->attributes[i]))
            return 2;
    }
    return 0;
}

int findSymbolInTable(symbol *table, int tableSize, char *symbolName){
    int i;
    for(i = 0 ; i < tableSize ; i++)
        if(!strcmp((table+i)->symbol, symbolName))
            return i;
    return -1;
}

int isExtern(char *inputLine){
    if(strstr(inputLine, EXTERN_DECLERATION))
        return 1;
    return 0;
}

int isEntry(char *inputLine){
    if(strstr(inputLine, ENTRY_DECLERATION))
        return 1;
    return 0;
}