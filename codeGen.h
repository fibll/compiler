#ifndef CODEGEN_H 
#define CODEGEN_H

#include "virtualCommands.h"

void writeToCode(short input);
void writeToCodeAtPosition(short input,char* pPosition);

// code function
int code(tCode virtualCommand, ...);

// writeToCode
// writeToCodeAtPosition
// openFile
// closeFile
// writeConstblock

#endif