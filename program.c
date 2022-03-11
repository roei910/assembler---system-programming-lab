#include "program.h"

int main(int argc, char **argv){
    checkInput(argc, argv);
    return 0;
}

void checkInput(int argc, char **argv){
    if(argc == 1)
        printf("[Error]: no input files\n");
    else
        startCompiler(argc, argv);
}

void startCompiler(int argc, char **argv){
    int i;
    for(i = 1; i < argc; i++)
        runProgram(*(argv+i));/*run each file name through compiler*/ 
}