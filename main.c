#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

// fix: MORPHEM.word don't stuck to 1024 chars (malloc and realloc)
// fix: all these global variables 

// global stuff

// edgeType
typedef enum EDGE_TYPE
{
    edgeNil = 0,  // NIL     
    edgeSymbol = 1,  // Symbol  
    edgeMorphem = 2,  // Morphem 
    egdeGraph = 4,  // Graph   
    edgeGraphEnd = 8,  // Graphende 
}edgeType;

// edges
typedef struct EDGE
{
    edgeType type;  
    
    union EDGE_DESCRIPTION 
    {
        unsigned long xVar; 
        int           symbol;
        //tMC           morphem;
        //tIdxGr        graphAdress; 
    }   edgeDescription;

    int (*fx)(void); 
    int nextEdge; 
    int alternativeEdge; 
}tBog;


static MORPHEM morph;



void parser(/*morphArray*/);
void block(/*morphArray*/);

int main (int argc, char* argv[])
{
	// Variabeln

    // open file to read
    file = fopen(argv[1], "r");
    
    // start lexer
    morph = lexer(file);

    // print all tokens
    /*
    // stop executing lexer if return value.id = -1
    while(morph.id != -1)
    {
        switch(morph.id)
        {
            case 0:
                printf("morph.word = %s\n", morph.word);
                break;
            case 1:
                printf("morph.symbol = %s\n", morph.symbol);
                break;
            default:
                printf("morph.number = %i\n", morph.number);
        }
        morph = lexer(file);
    }*/

    fclose(file);
    return 0;
}

void parser(/*morphArray*/)
{
    // verarbeiten des ersten Knotens, dessen kondition und weiter führende aktion
    // edge array
    
    //   nextEdge, alternative Edge
    // {
    //                              } 
}

void block(/*morphArray*/)
{

}
