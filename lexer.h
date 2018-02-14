#ifndef LEXER_H 
#define LEXER_H 

#include "types.h"
#include <stdlib.h>
#include <stdio.h>

// functions
int getCharClass();
MORPHEM lexer(FILE* file);
void init();
void nextInstruction(int state, int classType);
int read();
void write();
void writeCapital();
void beenden(int id);
void saveMorph(int id);
void debugLexerPrint(char* message);

#endif