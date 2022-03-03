#include "decodeLine.h"

int main(){
    /*
    int i;
    symbol *table = (symbol *) malloc(8 * sizeof(symbol));
    addSymbol(table, 0, "W", "external", 0, 0);
    addSymbol(table, 1, "MAIN", "code", 96, 4);
    addAttribute(table, 8, "MAIN", "entry");
    addSymbol(table, 2, "LOOP", "code", 96, 8);
    addSymbol(table, 3, "END", "code", 128, 12);
    addSymbol(table, 4, "STR", "data", 128, 13);
    addSymbol(table, 5, "LIST", "data", 144, 2);
    addAttribute(table, 8, "LIST", "entry");
    addSymbol(table, 6, "K", "data", 149, 5);
    addAttribute(table, 8, "K", "entry");
    addSymbol(table, 7, "val1", "external", 0, 0);
    for(i = 0 ; i < 8 ; i++){
        printSymbol(table+i);
    }
    */
    /*char inputLine[30] = "red r1";
    int x = checkValidOpperandsCommand(inputLine);
    printf("checking valid opperands: %s = ", inputLine);
    printf("%d\n", x);*/

    /*
    binLine line;
    char *binNumber;
    line.machineCode.code = 0;
    line.machineCode.word.A = 4;
    line.machineCode.code += 4; 
    numberToBinary(line.machineCode.code, &binNumber, SIZE_OF_WORD);
    printBinary(binNumber, SIZE_OF_WORD);
    printWord(line);*/
    binLine *lines = (binLine *) malloc(4 * sizeof(binLine));
    char inputLine[30] = "dec K";
    char command[MAX_COMMAND_NAME_LENGTH], dest[MAX_OPPERAND_LENGTH], src[MAX_OPPERAND_LENGTH];
    int valid, srcAddressing, destAddresing, destReg, srcReg;
    valid = decodeInstructionLine(inputLine, command, src, dest);
    switch (valid)
    {
        case 0:
            printf("valid command - no opperands\n");
            buildMachineCodeLines(lines, 100, command, 0);
            printWord(*lines);
            break;
        case 1:
            printf("valid command - 1 opperand, ");
            valid = checkValidCommandOneOpperand(command, dest, &destAddresing);
            if(valid){
                /*need to handle addressing to change dest to the right value*/
                printf("valid opperand\n");
                /*printf("%d lines\n", buildMachineCodeLines(lines, 100, command, 2, dest, destAddresing));*/
                if((destAddresing == 0) || (destAddresing == 3)){
                    if(destAddresing == 0)
                        destReg = getNumberFromOpperand(dest);
                    else
                        destReg = getRegFromOpperand(dest);
                    printf("%d lines\n", buildMachineCodeLines(lines, 100, command, 2, destReg, destAddresing));
                    printWord(*lines);
                    printWord(*(lines+1));
                    if(destAddresing == 0)
                        printWord(*(lines+2));
                }
                else{
                    printf("%d lines\n", buildMachineCodeLines(lines, 100, command, 2, destReg, destAddresing));
                    printWord(*lines);
                    printWord(*(lines+1));
                    printWord(*(lines+2));
                    printWord(*(lines+3));
                }
                
            }
            else
                printf("opperand does not fit the command\n");
            break;
        case 2:
            printf("valid command - 2 opperands, ");
            valid = checkValidCommandTwoOpperands(command, src, &srcAddressing, dest, &destAddresing);
            if(valid)
                printf("valid opperands\n");
            else
                printf("opperands do not fit the command\n");
            break;
        case -1:
            printf("number of opperands is incorrect\n");
            break;
        case -2:
            printf("illegal command name\n");
            break;
    }
    return 0;
}

void lineDecode(){

}

/**
 * @brief return true if the line is .data or .string
 * 
 * @param inputLine 
 * @return int 
 */
int isDataLine(char *inputLine){
    if(strstr(inputLine, DATA_COMMAND))
        return 1;
    if(strstr(inputLine, STRING_COMMAND))
        return 1;
    return 0;
}

/**
 * @brief return true if the line has a symbol
 * 
 * @param inputLine 
 * @return int 
 */
int isSymbol(char *inputLine){
    if(strstr(inputLine, LABEL_SYMBOL))
        return 1;
    return 0;
}

void addSymbol(symbol *table, int index, char *symbolName, char *attr ,int base, int offset){
    strcpy((table+index)->symbol, symbolName);
    (table+index)->baseAddress = base;
    (table+index)->offset = offset;
    strcpy(((table+index)->attributes)[0], attr);
    ((table+index)->attributeCount)++;
}

int addAttribute(symbol *table, int tableSize, char *symbolName, char *attr){
    int index = findSymbolInTable(table, tableSize, symbolName);
    int attrIndex;
    if(index == -1){
        printf("symbol not found\n");
        return 0;/*symbol not found in table*/
    }
    attrIndex = (table+index)->attributeCount;
    /*printf("index = %d, attrIndex = %d\n", index, attrIndex);*/
    if(attrIndex >= MAX_ATTRIBUTES)
        return -1;/*max attribues*/
    strcpy(((table+index)->attributes)[attrIndex], attr);
    ((table+index)->attributeCount)++;
    return 1;
}

int findSymbolInTable(symbol *table, int tableSize, char *symbolName){
    int i;
    for(i = 0 ; i < tableSize ; i++)
        if(!strcmp((table+i)->symbol, symbolName))
            return i;
    return -1;
}

int isExtern(char *inputLine){
    if(strstr(inputLine, EXTERN_COMMAND))
        return 1;
    return 0;
}

int isEntry(char *inputLine){
    if(strstr(inputLine, ENTRY_COMMAND))
        return 1;
    return 0;
}

void printSymbol(symbol *s){
    int i;
    printf("{symbol= %s, base= %d, offset= %d, attributes(%d)= ", s->symbol, s->baseAddress, s->offset, s->attributeCount);
    for(i = 0 ; i < s->attributeCount ; i++)
        printf("%s, ", s->attributes[i]);
    printf("}\n");
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
        ptr = strtok(inputLineCpy + strlen(command), COMMA_SYMBOL);
        sscanf(ptr, "%s", src);
        ptr = strtok(NULL, COMMA_SYMBOL);
        if(ptr != NULL)
            sscanf(ptr, "%s", dest);
        else{
            strcpy(dest, src);
            countOpp = 1;
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
    char reg[MAX_NAME], label[MAX_LABEL_LENGTH];
    if((ptr = strstr(opperand, LEFT_SQUARE_BRACKET))){
        sscanf(ptr+1, "%s", reg);/*get register from opperand*/
        ptr = strstr(reg, RIGHT_SQUARE_BRACKET);
        if(ptr != NULL)
            *ptr = '\0';/*replace ] with 0*/

        ptr = strtok(opperand, LEFT_SQUARE_BRACKET);
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
            return 0;
    }
    if(*destAddressing == 0){
        if(!strcmp(command, COMMAND_CMP))
            return 1;
        return 0;
    }
    return 1;
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

int checkValidOpperandsCommand(char *inputLine, char *command, char *src, char *dest){
    /*char src[MAX_OPPERAND_LENGTH], dest[MAX_OPPERAND_LENGTH], command[MAX_COMMAND_NAME_LENGTH];*/
    int valid, addressingTypeSrc, addressingTypeDest;
    valid = decodeInstructionLine(inputLine, command, src, dest);
    switch (valid)
    {
        case -1:/*incorrect number of opps*/
            return 0;
        case 0:
            return 1;
        case 1:
            addressingTypeDest = checkOpperandType(dest);
            if(addressingTypeDest == 0){
                if(!strcmp(command, COMMAND_PRN))
                    return 1;
                return 0;
            }
            else if(addressingTypeDest == 3){
                if(((!strcmp(command, COMMAND_JMP))) || (!strcmp(command, COMMAND_BNE)) ||
                    (!strcmp(command, COMMAND_JSR)))
                    return 0;
                return 1;
            }
            else if(addressingTypeDest == -1)
                return 0;
            return 1;
        case 2:
            addressingTypeDest = checkOpperandType(dest);
            addressingTypeSrc = checkOpperandType(src);
            if((addressingTypeSrc == 0) || (addressingTypeSrc == 3)){
                if(!strcmp(command, COMMAND_LEA))
                    return 0;
            }
            if(addressingTypeDest == 0){
                if(!strcmp(command, COMMAND_CMP))
                    return 1;
                return 0;
            }
            return 1;
        default:/*illegal command name*/
            return 0;
    }
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

int buildMachineCodeLines(binLine *lines, int IC, char *command, int arguments, ...){
    va_list valist;
    va_start(valist, arguments);
    int dest, destAddressing, src, srcAddressing;
    switch (arguments)
    {
        case 0:
            createBinaryLine(lines, 2, MACHINE_CODE_A, getOpcode(command));
            return 1;
        case 2:/*arguments: DEST REG, DEST ADDRESSING*/
            dest = va_arg(valist, int);
            destAddressing = va_arg(valist, int);
            createBinaryLine(lines, 2, MACHINE_CODE_A, (int)pow(2, getOpcode(command)));
            createBinaryLine(lines+1, 4, MACHINE_CODE_A, getFunct(command), dest, destAddressing);
            if(destAddressing == 0){
                createBinaryLine(lines+2, 2, MACHINE_CODE_A, dest);
                return 3;
            }
            if((destAddressing == 1) || (destAddressing == 2)){
                return 4;/*create two more empty lines*/
            }
            return 2;
        case 4:
            src = va_arg(valist, int);
            srcAddressing = va_arg(valist, int);

            dest = va_arg(valist, int);
            destAddressing = va_arg(valist, int);

            createBinaryLine(lines, 2, MACHINE_CODE_A, (int)pow(2, getOpcode(command)));
            createBinaryLine(lines+1, 6, MACHINE_CODE_A, getFunct(command), src, srcAddressing, dest, destAddressing);
            /*check how many lines to add*/
            return 2;
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

int writeSymbolLine(binLine *lines, int baseAddress, int offset){
    createBinaryLine(lines, 2, MACHINE_CODE_R, baseAddress);
    createBinaryLine(lines+1, 2, MACHINE_CODE_R, offset);
}