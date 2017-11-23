#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

// fix: MORPHEM.word don't stuck to 1024 chars (malloc and realloc)
// fix: all these global variables 

// global stuff
static MORPHEM morph;

// graph arrays
edge block[];
edge expression[];
edge term[];
edge statement[];
edge factor[];
edge condition[];

//  programm
edge programm[] = {
/*Num    type        ,           union         , action, next, alt */
/* 0*/  {edgeGraph   , {(unsigned long)block}  , NULL  ,   1 ,  0 },
/* 1*/  {edgeGraphEnd, {(unsigned long)0    }  , NULL  ,   0 ,  0 },
};


// block
edge block[] = {
/*Num    type       ,            union               , action, next, alt */
/* 0*/  {edgeSymbol , {(unsigned long)"CONST"}       , NULL  ,   1 ,  6 },
/* 1*/  {edgeMorphem, {(unsigned long)morphemIdent}  , NULL  ,   2 ,  0 },
/* 2*/  {edgeSymbol , {(unsigned long)'='}           , NULL  ,   3 ,  0 },
/* 3*/  {edgeMorphem, {(unsigned long)morphemNumeral}, NULL  ,   4 ,  0 },
/* 4*/  {edgeSymbol , {(unsigned long)','}           , NULL  ,   1 ,  5 },
/* 5*/  {edgeSymbol , {(unsigned long)';'}           , NULL  ,   7 ,  0 },
/* 6*/  {edgeNil    , {(unsigned long)0}             , NULL  ,   7 ,  0 },
/* 7*/  {edgeSymbol , {(unsigned long)"VAR"}         , NULL  ,   8 , 11 },
/* 8*/  {edgeMorphem, {(unsigned long)morphemIdent}  , NULL  ,   9 ,  0 },
/* 9*/  {edgeSymbol , {(unsigned long)','}           , NULL  ,   8 , 10 },
/*10*/  {edgeSymbol , {(unsigned long)';'}           , NULL  ,  12 ,  0 },
/*11*/  {edgeNil    , {(unsigned long)0}             , NULL  ,  12 ,  0 },
/*12*/  {edgeSymbol , {(unsigned long)"PROCEDURE"}   , NULL  ,  13 , 17 },
/*13*/  {edgeMorphem, {(unsigned long)morphemIdent}  , NULL  ,  14 ,  0 },
/*14*/  {edgeSymbol , {(unsigned long)';'}           , NULL  ,  15 ,  0 },
/*15*/  {edgeGraph  , {(unsigned long)block}         , NULL  ,  16 ,  0 },
/*16*/  {edgeSymbol , {(unsigned long)';'}           , NULL  ,  12 ,  0 },
/*17*/  {edgeNil    , {(unsigned long)0}             , NULL  ,  18 ,  0 },
/*18*/  {edgeGraph  , {(unsigned long)statement}     , NULL  ,   0 ,  0 },
};

// expression
edge expression[] = {
/*Num    type         ,            union               , action, next, alt */
/* 0*/  {edgeSymbol   , {(unsigned long)'-'}           , NULL  ,   2 ,  1 },
/* 1*/  {edgeNil      , {(unsigned long)0}             , NULL  ,   2 ,  0 },
/* 2*/  {edgeGraph    , {(unsigned long)term}          , NULL  ,   3 ,  0 },
/* 3*/  {edgeNil      , {(unsigned long)0}             , NULL  ,   4 ,  0 },
/* 4*/  {edgeSymbol   , {(unsigned long)'+'}           , NULL  ,   5 ,  6 },
/* 5*/  {edgeGraph    , {(unsigned long)term}          , NULL  ,   3 ,  0 },
/* 6*/  {edgeSymbol   , {(unsigned long)'-'}           , NULL  ,   7 ,  8 },
/* 7*/  {edgeGraph    , {(unsigned long)term}          , NULL  ,   3 ,  0 },
/* 8*/  {edgeGraphEnd , {(unsigned long)0}             , NULL  ,   0 ,  0 },
};

// term
edge term[] = {
/*Num    type         ,            union               , action, next, alt */
/* 0*/  {edgeGraph    , {(unsigned long)factor}        , NULL  ,   1 ,  0 },
/* 1*/  {edgeNil      , {(unsigned long)0}             , NULL  ,   2 ,  0 },
/* 2*/  {edgeSymbol   , {(unsigned long)'*'}           , NULL  ,   3 ,  4 },
/* 3*/  {edgeGraph    , {(unsigned long)factor}        , NULL  ,   1 ,  0 },
/* 4*/  {edgeSymbol   , {(unsigned long)'/'}           , NULL  ,   5 ,  6 },
/* 5*/  {edgeGraph    , {(unsigned long)factor}        , NULL  ,   1 ,  0 },
/* 6*/  {edgeGraphEnd , {(unsigned long)0}             , NULL  ,   0 ,  0 },
};


// statement
edge statement[] = {
/*Num    type         ,            union               , action, next, alt */
/* 0*/  {edgeMorphem  , {(unsigned long)morphemIdent}  , NULL  ,   1 ,  3 },
/* 1*/  {edgeSymbol   , {(unsigned long)":="}          , NULL  ,   2 ,  0 },
/* 2*/  {edgeGraph    , {(unsigned long)expression}    , NULL  ,   0 ,  0 },
/* 3*/  {edgeSymbol   , {(unsigned long)"IF"}          , NULL  ,   4 ,  7 },
/* 4*/  {edgeGraph    , {(unsigned long)condition}     , NULL  ,   5 ,  0 },
/* 5*/  {edgeSymbol   , {(unsigned long)"THEN"}        , NULL  ,   6 ,  0 },
/* 6*/  {edgeGraph    , {(unsigned long)statement}     , NULL  ,   0 ,  0 },
/* 7*/  {edgeSymbol   , {(unsigned long)"WHILE"}       , NULL  ,   8 , 11 },
/* 8*/  {edgeGraph    , {(unsigned long)condition}     , NULL  ,   9 ,  0 },
/* 9*/  {edgeSymbol   , {(unsigned long)"DO"}          , NULL  ,  10 ,  0 },
/*10*/  {edgeGraph    , {(unsigned long)statement}     , NULL  ,   0 ,  0 },
/*11*/  {edgeSymbol   , {(unsigned long)"BEGIN"}       , NULL  ,  12 , 15 },
/*12*/  {edgeGraph    , {(unsigned long)statement}     , NULL  ,  13 ,  0 },
/*13*/  {edgeSymbol   , {(unsigned long)';'}           , NULL  ,  12 ,  0 },
/*14*/  {edgeSymbol   , {(unsigned long)"END"}         , NULL  ,   0 ,  0 },
/*15*/  {edgeSymbol   , {(unsigned long)"CALL"}        , NULL  ,  16 , 17 },
/*16*/  {edgeMorphem  , {(unsigned long)morphemIdent}  , NULL  ,   0 ,  0 },
/*17*/  {edgeSymbol   , {(unsigned long)'?'}           , NULL  ,  18 , 19 },
/*18*/  {edgeMorphem  , {(unsigned long)morphemIdent}  , NULL  ,   0 ,  0 },
/*19*/  {edgeSymbol   , {(unsigned long)'!'}           , NULL  ,  20 , 21 },
/*20*/  {edgeGraph    , {(unsigned long)expression}    , NULL  ,   0 ,  0 },
/*21*/  {edgeGraphEnd , {(unsigned long)0}             , NULL  ,   0 ,  0 },
};


// factor
edge factor[] = {
/*Num    type         ,            union               , action, next, alt */
/* 0*/  {edgeMorphem  , {(unsigned long)morphemNumeral}, NULL  ,   0 ,  1 },
/* 1*/  {edgeSymbol   , {(unsigned long)'('}           , NULL  ,   2 ,  4 },
/* 2*/  {edgeGraph    , {(unsigned long)expression}    , NULL  ,   3 ,  0 },
/* 3*/  {edgeNil      , {(unsigned long)0}             , NULL  ,   0 ,  0 },
/* 4*/  {edgeMorphem  , {(unsigned long)morphemIdent}  , NULL  ,   0 ,  0 },
};


// condition
edge condition[] = {
/*Num    type         ,            union               , action, next, alt */
/* 0*/  {edgeSymbol   , {(unsigned long)"ODD"}         , NULL  ,   1 ,  2 },
/* 1*/  {edgeGraph    , {(unsigned long)expression}    , NULL  ,   0 ,  0 },
/* 2*/  {edgeGraph    , {(unsigned long)expression}    , NULL  ,   3 ,  0 },
/* 3*/  {edgeSymbol   , {(unsigned long)'='}           , NULL  ,   9 ,  4 },
/* 4*/  {edgeSymbol   , {(unsigned long)'#'}           , NULL  ,   9 ,  5 },
/* 5*/  {edgeSymbol   , {(unsigned long)'<'}           , NULL  ,   9 ,  6 },
/* 6*/  {edgeSymbol   , {(unsigned long)'>'}           , NULL  ,   9 ,  7 },
/* 7*/  {edgeSymbol   , {(unsigned long)"<="}          , NULL  ,   9 ,  8 },
/* 8*/  {edgeSymbol   , {(unsigned long)">="}          , NULL  ,   9 ,  0 },
/* 9*/  {edgeGraph    , {(unsigned long)expression}    , NULL  ,   0 ,  0 },
};


int parser(edge* graph);

int main (int argc, char* argv[])
{
	// Variabeln
    // init morphem
    /*
    morph.id = -1;
    morph.word = "";
    morph.symbol = 0;
    morph.number = 0;
    morph.keyword = "";
    */

    // is there an argument?
    if(argc < 2)
    {
        printf("Fehlendes Argument: PL0 Datei!\n");
        exit(-1);
    }

    // open file to read
    file = fopen(argv[1], "r");
    if(file == NULL)
    {
        printf("Datei kann nicht geöffnet werden!\n");
        exit(-1);
    }
    
    // init lexer one time only
    read();

    // start lexer
    morph = lexer(file);

    // print all tokens

    // stop executing lexer if return value.id = -1
    while(morph.id != -1)
    {
        switch(morph.id)
        {
            case 0:
                printf("morph.word = %s\n", morph.word);
                break;
            case 1:
                printf("morph.allSymbol = %i\n", morph.allSymbols);
                break;
            case 2:
                printf("morph.number = %i\n", morph.number);
                break;
            default:
                printf("ERROR!");
                exit(0);

        }
        morph = lexer(file);
    }


    fclose(file);
    return 0;
}

int parser(edge* graph)
{
    // function is executed for every single edge
    
    // init success variable

    // get token if lexer wasn't used yet or the old one was already processed
    
    // switch case with different edge types

    // nil edge (0):
        // always success
    
    // Symbol edge (1):
        // check if the symbol is equal to the morph.Symbol
        // success = true;
        // success = false;

    // Morphem edge(2):
        // check if the morphem code (ident or number) is equal to morph.id of the token
        // success = true;
        // success = false;
        // clear the token, so the next parser action can get it's own new next token

    // Graph edge (3):
        // success is the return value of the parse function with the given graph 
        // success = parse(graph)

    // Graph End edge (4):
        // return success

    // find out what action to do
    // if null, success = 1, else success is defined by action

    // is success true?
        //no: if alternative edge is not 0 (otherwise exit without success)
            // try alternative edge

        //yes: go further with the next edge
}
