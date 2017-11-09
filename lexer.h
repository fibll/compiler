#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// functions
int getCharClass();
void lexer(char* fileName);
void init();
void nextInstruction(int state, int class);
void write();
void writeCapital();
void beenden();



void lexer(char* fileName)
{
    // open file to read
    file = fopen(fileName, "r");
    
    // read in first char
    int class = read();
        
    while(input != 59)
    {
        // call init for a new token
        init();
        // print token with linebreak
    }
}

void init()
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
    if (input > 127)
        return 7;
    else
        return charClasses[input];
}

void nextInstruction(int state, int class)
{
    // printf("state: %i, class %i", state, class);

   // look up which state is next depending on the given state and char class
   int nextState = stateAction[state][class][1];
   int toDo = stateAction[state][class][0];
   int newClass = 0;

   // call toDo and call nextInstruction with new state
   switch(toDo)
   {
