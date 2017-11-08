#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// fix: MORPHEM.word don't stuck to 30 chars
// fix: all these global variables 



// global stuff
typedef struct morph
{
    int id;
    char* word;
    char symbol[2];
    double number;
}MORPHEM;

static MORPHEM morph;

// Zeichenklassenvector
static char charClasses[128]=
/*      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F     */
/*­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­*/
/* 0*/{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,/* 0*/
/*10*/ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,/*10*/
/*20*/ 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,/*20*/
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
static int stateAction[8][9][2] = {
// soZei      Ziffer     Buchst       :          =          <          >        Sonst  
  4 , -1  ,  2 ,  2  ,  3 ,  1  ,  2 ,  3  ,  4 , -1  ,  2 ,  4  ,  2 ,  5  ,  1 , -1  ,  5 ,  0  ,
  1 , -1  ,  2 ,  1  ,  3 ,  1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  2 ,  2  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  2 ,  6  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  2 ,  7  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  2 ,  8  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
};

char* pointer = NULL;


// functions
int getCharClass();
void lexer();
void init();
void nextInstruction(int state, int class);
void schreiben();
void grossSchreiben();
void beenden();

int main (int argc, char* argv[])
{
	// Variabeln
    // call lexer function with given argument
    pointer = argv[1];
    lexer();

	return 0;
}

void lexer()
{
  //while loop until eof
  {
    // call z0 with input, toDo = 0 and get token back
    init();
    // print token with linebreak
  }
}

// sollte eigentlich intiLexer sein und der rest sollte wie die anderen Zustände behandelt werden!!!
// Leerzeichen werden normal behandelt
void init()
{
    // read first sign and tell which class it is
    int class = lesen();

    // call nextInstruction with given char class
    // printf("Class: %i", getCharClass());
    nextInstruction(0, getCharClass());
}

int getCharClass()
{
    if (*pointer > 127)
        return 7;
    else
        return charClasses[*pointer];
}

void nextInstruction(int state, int class)
{
   // look up which state is next depending on the given state and char class
   int nextState = stateAction[state][class][1];
   int toDo = stateAction[state][class][0];
   int newClass = 0;

   // call toDo and call nextInstruction with new state
   switch(toDo)
   {
       case 1: printf("beenden\n");
           break;
       case 2: schreiben();
               newClass = lesen();
               nextInstruction(nextState, newClass);
          break;
       case 3: grossSchreiben();
               newClass = lesen();
               nextInstruction(nextState, newClass);
          break;
       case 4: schreiben();
               newClass = lesen();
               beenden();
          break;
       case 5: newClass = lesen();
               nextInstruction(nextState, newClass);
          break;
       default: printf("Something did go wrong!\n");
   }
}

int lesen()
{
    printf("lesen\n");
    pointer++;
    // if "\n"
        // schreiben("\n");
        // return 7;
    // else
        return getCharClass();
}

void schreiben()
{
    printf("schreiben\n");
    //morph.word = strcat(morph.word,pointer);
    //printf("%s\n", morph.word);
}

void grossSchreiben()
{
    printf("großschreiben\n");
    //morph.word = strcat(morph.word,"bla");
    //printf("%s\n", morph.word);
}

void beenden()
{
    printf("beenden");
}
