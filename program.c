/**
 * @file program.c
 * @author roei daniel
 * @brief run the compiler program from the command line
 * can read multiple files to be compiled
 */
#include "program.h"

/**
 * @brief main function, checks if the function received arguments
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char **argv){
    if(argc == 1)
        printf("[Error]: no input files\n");
    else
        startCompiler(argc, argv);
    return 0;
}

/**
 * @brief start to run compiler program for each file received
 * 
 * @param argc 
 * @param argv 
 */
void startCompiler(int argc, char **argv){
    int i;
    for(i = 1; i < argc; i++)
        runProgram(*(argv+i));/*run each file name through compiler*/ 
}