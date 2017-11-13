#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// fix: MORPHEM.word don't stuck to 1024 chars (malloc and realloc)
// fix: all these global variables 
// todo: save tokens
// todo: save tokens with identifier (symbol, word, number)
// todo: writeCapital: save just capital letters
    // letters that are already capital could fuck up
// todo: array of tokens?

// global stuff
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

int input = 0;
char tokenBuffer[1024];
char* tokenPointer = &tokenBuffer[0];

typedef struct morph
{
    int id;
    char word[30];
    char symbol[2];
    double number;
}MORPHEM;

static MORPHEM morph;

MORPHEM* morphArray;

size_t morphArraySize = 0;

FILE* file = NULL;

// functions
int getCharClass();
void lexer();
void init();
void nextInstruction(int state, int class);
void write();
void writeCapital();
void beenden();

int main (int argc, char* argv[])
{
	// Variabeln

    // open file to read
    file = fopen(argv[1], "r");

    morphArray = (MORPHEM*)malloc(0 * sizeof(MORPHEM));

    //morphArray = (MORPHEM*) realloc(morphArray, 17 * sizeof(MORPHEM));
    //printf("\ntest\n");

    // init with first char
    read();
        
    while(input != -1)
    {
        // call init for a new token
        lexer();
        // print token with linebreak
    }

    // print all tokens
    int i = 0;

    for(i = 0; i < morphArraySize;i++)
    {
        printf("ASDDmorphArray[%i].word = %s\n", i, morphArray[i].word);
    }

    printf("free\n");
    free(morphArray);

    return 0;
}

void lexer()
{
    if(file != NULL)
    {
        // call nextInstruction with given char class
        nextInstruction(0, getCharClass());
    }
    else
    {
        printf("file could not been opened");
        exit(0);
    }
}

int getCharClass()
{
    //printf("input: %i\n", input);

    if (input >= 127 || input < 0)
    {
        printf("Failure: Unaccapted character!\n");
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
       case 1: beenden();
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
               beenden();
          break;

       // read
       case 5: newClass = read();
               nextInstruction(nextState, newClass);
          break;
       default: printf("Something did go wrong!\n");
                input = -1;
   }
}

int read()
{
    //printf("read\n");
    
    input = fgetc(file);
    
    return getCharClass();
}

void write()
{
    //printf("write\n");

    *tokenPointer = input;
    tokenPointer++;
    //printf("%s\n", tokenBuffer);
}

void writeCapital()
{
    //printf("write capital\n");
    if(input > 90)
        input -= 32;

    *tokenPointer = input;
    

    tokenPointer++;
    //printf("%s\n", tokenBuffer);
}

void beenden()
{
    int index = sizeof(morphArray)-1;

    printf("beenden\n");
    printf("\n\n----------------------\nToken: %s\n\n\n", tokenBuffer);

    // create out of the tokenBuffer a new morphem
    morphArraySize++;
    morphArray = (MORPHEM*) realloc(morphArray, morphArraySize * sizeof(MORPHEM));
    strcpy(morphArray[morphArraySize-1].word, tokenBuffer);
    printf("Saved in morphArray\n");

    // empty tokenBuffer
    memset(tokenBuffer, 0, strlen(tokenBuffer));
    //tokenBuffer = "";

    // set tokenPointer to beginnig of tokenBuffer
    tokenPointer = &tokenBuffer[0];
}
