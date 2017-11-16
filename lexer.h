#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// fix: MORPHEM.word don't stuck to 1024 chars (malloc and realloc)
// fix: all these global variables 

// global stuff
int debug = 0;


// morphem
typedef struct morph
{
    int id;
    char word[30];
    char symbol[2];
    int number;
    char keyword[30];
}MORPHEM;

static MORPHEM morph;

// Zeichenklassenvector
static char charClasses[128]=
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
static int stateAction[8][8][2] = {
// soZei      Ziffer     Buchst       :          =          <          >        Sonst
  4 , -1  ,  2 ,  2  ,  3 ,  1  ,  2 ,  3  ,  4 , -1  ,  2 ,  4  ,  2 ,  5  ,  5 ,  0  ,
  1 , -1  ,  2 ,  1  ,  3 ,  1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  2 ,  2  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  2 ,  6  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  2 ,  7  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  2 ,  8  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
};


int keywordFirstLetter[] = {
/*  a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z*/
    -1,0, 1, 2, 3,-1,-1,-1, 4,-1,-1,-1,-1,-1, 5, 6,-1,-1,-1, 7,-1, 8, 9,-1, -1,-1 
};

int keywordLength[] = {
    5, -1, 2, 3, 2, 3, 9, 4, 3, 5
};


// keyword array
char* keywords[11]/*[2]*/ = {
    "BEGIN","CALL","CONST","DO","END","IF","ODD","PROCEDURE","THEN","VAR","WHILE"
        /*,
      "0"  , "1",    "2",   "3", "4",  "5", "6",  "7",        "8",   "9",  "10"  */
};


int input = 0;
char tokenBuffer[1024];
char* tokenPointer = &tokenBuffer[0];


FILE* file = NULL;

// functions
int getCharClass();
MORPHEM lexer(FILE* file);
void init();
void nextInstruction(int state, int class);
void write();
void writeCapital();
void beenden(int id);
void saveMorph(int id);
void debugPrint(char* message);

MORPHEM lexer(FILE* file)
{
    // init with first char
    //read();
   
    if(file != NULL)
    {
        // call nextInstruction with given char class
        nextInstruction(0, getCharClass());
    }
    else
    {
        debugPrint("file could not been opened");
        exit(0);
    }

    return morph;
}

int getCharClass()
{
    //printf("input: %i\n", input);

    if (input >= 127 || input < 0)
    {
        debugPrint("Failure: Unaccapted character!\n");
        input = -1;
        return -1;
    }
    else
        return charClasses[input];
}

void nextInstruction(int state, int class)
{
   //printf("state: %i, class: %i\n", state, class);

   // catch eof and unaccapted characters
   if(class < 0)
   {
       morph.id = -1;
       return;
   }

   // look up which state is next depending on the given state and char class
   int nextState = stateAction[state][class][1];
   int toDo = stateAction[state][class][0];
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
       default: debugPrint("Something did go wrong!\n");
                input = -1;
   }
}

int read()
{
    //debugPrint("read\n");
    
    input = fgetc(file);
    
    return getCharClass();
}

void write()
{
    //debugPrint("write\n");

    *tokenPointer = input;
    tokenPointer++;
    //printf("%s\n", tokenBuffer);
}

void writeCapital()
{
    //debugPrint("write capital\n");
    if(input > 90)
        input -= 32;

    *tokenPointer = input;
    

    tokenPointer++;
    //printf("%s\n", tokenBuffer);
}

void beenden(int id)
{
    debugPrint("beenden\n");
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

    // create out of the tokenBuffer a new morphem
    //morphArraySize++;
    //morphArray = (MORPHEM*) realloc(morphArray, morphArraySize * sizeof(MORPHEM));
   
    if(id < 0 || id > 8)
    {
        debugPrint("Something did go wrong");
        morph.id = -1;
        input = -1;
        return;
    }

    switch(id)
    {
        // word
        case 1:
                // check if keyword
                firstChar = tokenBuffer[0];
                //printf("firstChar: %c\n", firstChar);
                
                // check with the first letter if it could be a keyword
                lengthOK = keywordFirstLetter[firstChar-65]; 
                //printf("\n\nstrlen: %i\n", strlen(tokenBuffer));

                // check if firstLetter is correct
                if(lengthOK > -1)
                {
                    if(lengthOK == 1 && strlen(tokenBuffer) > 3 && strlen(tokenBuffer) < 6)
                    {
                        keyword = 1;
                    }
                    else if(strlen(tokenBuffer) == keywordLength[lengthOK])
                    {
                        keyword = 1;
                    }
                }
                

                if(keyword == 1)
                {
                    printf("\n\npotential keyword\n");
                    
                    int i = 0;

                    // find keyword in the keyword array
                    for(i = 0; i < 11; i++)
                    {
                        if(strcmp(tokenBuffer, keywords[i]) == 0)
                        {
                            morph.id = 3;
                            strcpy(morph.keyword, tokenBuffer);
                            i = 11;
                        }
                        else
                        {
                            morph.id = 0;
                            strcpy(morph.word, tokenBuffer);
                        }
                    }
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
                strcpy(morph.symbol, tokenBuffer);
            break;
    }
     
    debugPrint("Saved in morph\n");
    
}
void debugPrint(char* message)
{
    if(debug > 0)
    {
        printf("%s", message);
    }
    return;
}
