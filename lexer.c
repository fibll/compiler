#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "lexer.h"

// global stuff
MORPHEM morph;
size_t lineCounter;

// files
FILE* file;

// lexer specific
int input = 0;
char tokenBuffer[1024];
char* tokenPointer = &tokenBuffer[0];
int debugLexer;

// Zeichenklassenvector
char charClasses[128]=
/*      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F     */
/*­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­*/
/* 0*/{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,/* 0*/
/*10*/ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,/*10*/
/*20*/ 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,/*20*/
/*30*/ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 5, 4, 6, 0,/*30*/
/*40*/ 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,/*40*/
/*50*/ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0,/*50*/
/*60*/ 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,/*60*/
/*70*/ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0}/*70*/;

// sglsb
//  1 b
//  2 s l
//  3 sg l
//  4 s l b
//  5 l
//
// cur. state|input|[lsb, next state]
int stateAction[9][8][2] = {
//       soZei      Ziffer     Buchst       :          =          <          >        Sonst
/*Z0*/  4 , -1  ,  2 ,  2  ,  3 ,  1  ,  2 ,  3  ,  4 , -1  ,  2 ,  4  ,  2 ,  5  ,  5 ,  0  ,
/*Z1*/  1 , -1  ,  2 ,  1  ,  3 ,  1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
/*Z2*/  1 , -1  ,  2 ,  2  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
/*Z3*/  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  2 ,  6  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
/*Z4*/  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  2 ,  7  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
/*Z5*/  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  2 ,  8  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
/*Z6*/  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
/*Z7*/  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
/*Z8*/  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
};


int keywordFirstLetter[] = {
/*  a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z*/
    -1,0, 1, 2, 3,-1,-1,-1, 4,-1,-1,-1,-1,-1, 5, 6,-1,-1,-1, 7,-1, 8, 9,-1, -1,-1 
};

int keywordLength[] = {
    5, -1, 2, 3, 2, 3, 9, 4, 3, 5
};


// keyword array
char* keywords[12]/*[2]*/ = {
    "BEGIN","","DO","END","IF","ODD","PROCEDURE","THEN","VAR","WHILE","CALL","CONST"
        /*,
      "0"  , "1",    "2",   "3", "4",  "5", "6",  "7",        "8",   "9",  "10"  */
};

MORPHEM lexer(FILE* file)
{
    // init with first char
    //read();
   
    if(file != NULL)
    {
        // call nextInstruction with given char classType
        nextInstruction(0, getCharClass());
    }
    else
    {
        debugLexerPrint("file could not been opened");
        exit(0);
    }

    return morph;
}

int getCharClass()
{
    //printf("input: %i\n", input);

    if (input >= 127 || input < 0)
    {
        debugLexerPrint("Failure: Unaccapted character!\n");
        input = -1;
        return -1;
    }
    else
        return charClasses[input];
}

void nextInstruction(int state, int classType)
{
   //printf("state: %i, classType: %i\n", state, classType);

   // catch eof and unaccapted characters
   if(classType < 0)
   {
       morph.id = -1;
       return;
   }

   // look up which state is next depending on the given state and char classType
   int nextState = stateAction[state][classType][1];
   int toDo = stateAction[state][classType][0];
   int newClass = 0;

   // call toDo and call nextInstruction with new state
   switch(toDo)
   {
       // beenden
       case 1: beenden(state);
           break;

       // write read 
       case 2: write();
               newClass = read();
               nextInstruction(nextState, newClass);
          break;

       // write capital read
       case 3: writeCapital();
               newClass = read();
               nextInstruction(nextState, newClass);
          break;

       // write read beenden
       case 4: write();
               newClass = read();
               beenden(state);
          break;

       // read
       case 5: newClass = read();
               nextInstruction(nextState, newClass);
          break;
       default: debugLexerPrint("Something did go wrong!\n");
                input = -1;
   }
}

int read()
{
    //debugLexerPrint("read\n");
    printf(".");

    input = fgetc(file);

    // printf("input: %i\n", input);

    // if input = LF increase lineCounter
    if(input == 10){
        lineCounter++;
        // printf("\n\nLINE %i ===\n", lineCounter);
    }
    
    return getCharClass();
}

void write()
{
    //debugLexerPrint("write\n");

    *tokenPointer = input;
    tokenPointer++;
    //printf("%s\n", tokenBuffer);
}

void writeCapital()
{
    //debugLexerPrint("write capital\n");
    if(input > 90)
        input -= 32;

    *tokenPointer = input;
    

    tokenPointer++;
    //printf("%s\n", tokenBuffer);
}

void beenden(int id)
{
    debugLexerPrint("beenden\n");
    //printf("\n\n----------------------\nToken: %s\n\n\n", tokenBuffer);

    saveMorph(id);

    // empty tokenBuffer
    memset(tokenBuffer, 0, strlen(tokenBuffer));
    //tokenBuffer = "";

    // set tokenPointer to beginnig of tokenBuffer
    tokenPointer = &tokenBuffer[0];

    // start new token, if input is not < 0
    //if(input > -1)
    //    nextInstruction(0, getCharClass());

    return;
}

void saveMorph(int id)
{
    char firstChar = 0;
    int lengthOK = 0;
    int keyword = 0;

    if(id < 0 || id > 8)
    {
        debugLexerPrint("Something did go wrong");
        morph.id = -1;
        input = -1;
        return;
    }

    // set line number
    morph.lineNumber = lineCounter;

    switch(id)
    {
        // word
        case 1:
                // check if keyword
                firstChar = tokenBuffer[0];
                
                // check with the first letter if it could be a keyword
                lengthOK = keywordFirstLetter[firstChar-65]; 

                // check if firstLetter is correct
                if(lengthOK > -1)
                {
                    // if word starts with c
                    if(lengthOK == 1 && strlen(tokenBuffer) > 3 && strlen(tokenBuffer) < 6)
                    {
                        if(strcmp(tokenBuffer, keywords[10]) == 0)
                        {
                            keyword = 1;
                            morph.symbol = 131 + 10;
                        }
                        else if(strcmp(tokenBuffer, keywords[11]) == 0)
                        {
                            keyword = 1;
                            morph.symbol = 131 + 11;
                        }
                        else
                            keyword = 0;
                    }
                    // any other of the keyword letters
                    else if(strlen(tokenBuffer) == keywordLength[lengthOK])
                    {
                        if(strcmp(tokenBuffer, keywords[lengthOK]) == 0)
                        {
                            keyword = 1;
                            morph.symbol = 131 + lengthOK;
                        }
                        else
                            keyword = 0;
                    }
                }

                if(keyword == 1)
                {
                    morph.id = 1;
                }
                else
                {
                    morph.id = 0;
                    strcpy(morph.word, tokenBuffer);
                }
            break;
        // number
        case 2:
                morph.id = 2;
                morph.number = atoi(tokenBuffer);
            break;
        // symbol
        default:
                morph.id = 1;

                if(strlen(tokenBuffer) > 1)
                {
                    // switch for double symbol detection
                    if(strcmp(tokenBuffer,"<=") == 0)
                        morph.symbol = 128;
                    else if(strcmp(tokenBuffer,">=") == 0)
                        morph.symbol = 129;
                    else if(strcmp(tokenBuffer,":=") == 0)
                        morph.symbol = 130;
                }
                else
                    morph.symbol = tokenBuffer[0];
    }

    debugLexerPrint("Saved in morph\n");
}

void debugLexerPrint(char* message)
{
    if(debugLexer > 0)
    {
        printf("%s", message);
    }
    return;
}
