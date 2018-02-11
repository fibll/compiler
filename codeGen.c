#include "codeGen.h"
#include "namelist.h"
#include "stdarg.h"
#include <stdlib.h>
#include <stdio.h>

static int constArraySize;
static long *constArray;
static namelistProcedure *currentProcedure;

// extra
static int indexCodeOutput;
static int indexProcedure;
static int codeMemoryRange;
static char* codeStartAdress;
static char* pCode;
static FILE* outputFile;



// Functions ===========================================================
// openFile
// writeCodeToFile
int writeCodeToFile(){

    // calculate codeOutputLength
    short codeOutputLength = pCode - codeStartAdress;

    printf("%i", codeOutputLength);

    // update the procedure length at the front (initialized with 0)
    writeToCodeAtPosition(codeOutputLength, codeStartAdress + 1);

    // check if file open
    if(outputFile == NULL){
        printf("Error: writeToFile: output file is not open!\n");
        exit(EXIT_FAILURE);
    }

    // write code
    if(fwrite(codeStartAdress, sizeof(char), codeOutputLength, outputFile) != codeOutputLength){
        printf("Error: writeToFile: writing into output file did not work!\n");
        exit(EXIT_FAILURE);
    }

    // clear the memory
    pCode++;
    codeStartAdress = pCode;

    // reset codeMemoryRange

    return 1;
}



void writeToCode(short input){
    *pCode++=(unsigned char)(input & 0xff);
    *pCode++=(unsigned char)(input >> 8);
}

void writeToCodeAtPosition(short input, char *pPosition){
  * pPosition      = (unsigned char)(input & 0xff);
  *(pPosition + 1) = (unsigned char)(input >> 8);
}

int code(tCode virtualCommand, ...){
    // get argument list
    va_list arguments;
    short currentArgument;

    // check if memory space is not enough
    if(pCode - (codeStartAdress + MAX_LEN_OF_CODE) >= codeMemoryRange){

        // add 1024 to codeMemoryRange
        char* pTemp = realloc(codeStartAdress, (codeMemoryRange += 1024));
        if(pTemp == NULL){
            printf("Error: CodeGeneration: Was not possible to realloc memory for codeStartAdress\n");
            exit(EXIT_FAILURE);
        }
        
        // get the same offset of pCode to startAdress as before
        pCode = pTemp + (pCode - codeStartAdress);

        // init codeStartAdress
        codeStartAdress = pTemp;
    }

    // initialize pCode with virtualCommand and increase it
    if(pCode == NULL){
        printf("Error: CodeGeneration: pCode is NULL\n");
        exit(EXIT_FAILURE);
    }

    *pCode = (char) virtualCommand;
    pCode++;

    // set the start of the arguments with va_start (function of stdarg.h) and last ordinary argument (virtualCommand)
    va_start(arguments, virtualCommand);


    switch(virtualCommand){

        // commands with 3 arguments
        case entryProc:
            // get out an argument of type int
            currentArgument = va_arg(arguments, int);
            writeToCode(currentArgument);

        // commands with 2 arguments
        case puValVrGlob:
        case puAdrVrGlob:
            currentArgument = va_arg(arguments, int);
            writeToCode(currentArgument);

        // commands with 1 arguments
        case puValVrMain:
        case puAdrVrMain:
        case puValVrLocl:
        case puAdrVrLocl:
        case puConst:
        case jmp:
        case jnot:
        case call:
            currentArgument = va_arg(arguments, int);
            writeToCode(currentArgument);
            break;

        default:
            break;
    }

    va_end(arguments);
    return 1;
}