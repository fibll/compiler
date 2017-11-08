#include <stdio.h>

//void lexer(char* text);

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
//
// cur. state|input|[lsb, next state]
static int stateAction[8][8][2] = {
// soZei      Ziffer     Buchst       :          =          <          >        Sonst  
  4 , -1  ,  2 ,  2  ,  3 ,  1  ,  2 ,  3  ,  4 , -1  ,  2 ,  4  ,  2 ,  5  ,  1 , -1  ,
  1 , -1  ,  2 ,  1  ,  3 ,  1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  2 ,  2  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  2 ,  6  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  2 ,  7  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  2 ,  8  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,  1 , -1  ,
};

char* pointer = NULL;

int getCharClass();
void lexer();
void z0(int toDo);
int nextInstruction(int state, int class);

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
    z0(0);
    // print token with linebreak
  }
}

void z0(int toDo)
{
    int class = 0;

    if(toDo == 0)
    {
        while(*pointer == 32) // and tab
        {
            printf("space\n");
            pointer++;
            // set pointer to next char
        }//current char is space or tab
        
        // find out which class the current char is
                                    // if class is null we could catch it here
        // call nextInstruction with given char class
        nextInstruction(0, getCharClass());
    }
    else
        // execute toDo with function array?
        printf("execute toDo\n");
}

/*
z1(input, toDo(lesen, schreiben, beenden))
{
    // execute toDo
    // find out which class the current char is 
    // call nextInstruction with given char class
}
*/

int getCharClass()
{
    if (*pointer > 127)
        return 0;
    else
        return charClasses[*pointer];
}

int nextInstruction(int state, int class)
{
   // look up which state is next depending on the given state and char class
   printf("look up state for state %i char %c with class %i\n", state, *pointer, class);
   printf("Next state: %i\nAction: %i", stateAction[state][class][1], stateAction[state][class][0]);
   // call next state with input, toDo instruction
}
