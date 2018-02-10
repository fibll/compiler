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
static int codeOutputLength;
static char* codeStartAdress;
static char* pCode;


// Functions ===========================================================
// openFile
// writeCodeToFile
int writeCodeToFile(char* fileName){
    FILE* file = NULL;

    // open file
    file = fopen(fileName, "w");

    char test[] = "writeTest!";

    fwrite(test, sizeof(char), strlen(test), file);

    char test2[] = "more\n";

    fwrite(test2, sizeof(char), strlen(test2), file);

    char test3[] = "after line break\n";

    fwrite(test3, sizeof(char), strlen(test3), file);

    // close file
    fclose(file);
}



void writeToCode(short input){
    *pCode++=(unsigned char)(input & 0xff);
    *pCode++=(unsigned char)(input >> 8);
}

void writeToCodeAtPosition(short input,char* pPosition){
  * pPosition      = (unsigned char)(input & 0xff);
  *(pPosition + 1) = (unsigned char)(input >> 8);
}

int code(tCode virtualCommand, ...){
    // get argument list
    va_list arguments;
    short currentArgument;

    // check if memory space is not enough
    if(pCode - (codeStartAdress + MAX_LEN_OF_CODE) >= codeOutputLength){
        char* pTemp = realloc(codeStartAdress, (codeOutputLength += 1024));
        if(pTemp == NULL){
            printf("Error: CodeGeneration: Was not possible to realloc memory for codeStartAdress\n");
            exit(EXIT_FAILURE);
        }
        
        // get the same offset of pCode to startAdress as before
        pCode = pTemp + (pCode - codeStartAdress);
        codeStartAdress = pTemp;
    }

    // increase pCode and initialize it with virtualCommand
    *pCode++ = (char) virtualCommand;

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