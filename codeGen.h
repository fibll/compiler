#ifndef CODEGEN_H 
#define CODEGEN_H

#include "virtualCommands.h"

int writeCodeToFile();
void writeToCode(short input);
void writeToCodeAtPosition(short input, char *pPosition);
// code function
int code(tCode virtualCommand, ...);

// openFile
// closeFile
// writeConstblock

#endif