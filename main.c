#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "namelist.c"
#include "codeGen.c"
#include "semanticRoutine.c"

// fix: MORPHEM.word don't stuck to 1024 chars (malloc and realloc)
// fix: all these global variables 

// global stuff
static MORPHEM morph;
static int constArraySize = 0;
static int32_t *constArray;
static namelistProcedure *currentProcedure;
static short codeLength;
static labellist *labelList;
static FILE* outputFile;
static short procedureCounter;

// extra
static int indexCodeOutput;
static int indexProcedure;
static int codeMemoryRange;
static char* codeStartAdress;
static char* pCode;

//int debugParser = 1;
static namelistProcedure *mainProcedure;
static size_t lineCounter;

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
/* 1*/  {edgeGraphEnd, {(unsigned long)'.'}    , NULL  ,   0 ,  0 },
};


// block
edge block[] = {
/*Num    type       ,            union               , action, next, alt */
/* 0*/  {edgeSymbol , {(unsigned long)CONST}         , NULL  ,   1 ,  6 },
/* 1*/  {edgeMorphem, {(unsigned long)morphemIdent}  , blockAcceptConstantIdentifier  ,   2 ,  0 },
/* 2*/  {edgeSymbol , {(unsigned long)'='}           , NULL  ,   3 ,  0 },
/* 3*/  {edgeMorphem, {(unsigned long)morphemNumeral}, blockAcceptConstantValue  ,   4 ,  0 },
/* 4*/  {edgeSymbol , {(unsigned long)','}           , NULL  ,   1 ,  5 },
/* 5*/  {edgeSymbol , {(unsigned long)';'}           , NULL  ,   7 ,  0 },
/* 6*/  {edgeNil    , {(unsigned long)0}             , NULL  ,   7 ,  0 },
/* 7*/  {edgeSymbol , {(unsigned long)VAR}           , NULL  ,   8 , 11 },
/* 8*/  {edgeMorphem, {(unsigned long)morphemIdent}  , blockAcceptVariable  ,   9 ,  0 },
/* 9*/  {edgeSymbol , {(unsigned long)','}           , NULL  ,   8 , 10 },
/*10*/  {edgeSymbol , {(unsigned long)';'}           , NULL  ,  12 ,  0 },
/*11*/  {edgeNil    , {(unsigned long)0}             , NULL  ,  12 ,  0 },
/*12*/  {edgeSymbol , {(unsigned long)PROCEDURE}     , NULL  ,  13 , 17 },
/*13*/  {edgeMorphem, {(unsigned long)morphemIdent}  , blockAcceptProcedure  ,  14 ,  0 },
/*14*/  {edgeSymbol , {(unsigned long)';'}           , NULL  ,  15 ,  0 },
/*15*/  {edgeGraph  , {(unsigned long)block}         , NULL  ,  16 ,  0 },
/*16*/  {edgeSymbol , {(unsigned long)';'}           , NULL  ,  12 ,  0 },
/*17*/  {edgeNil    , {(unsigned long)0}             , blockstartStatement  ,  18 ,  0 },
/*18*/  {edgeGraph  , {(unsigned long)statement}     , blockEndOfProcedureDescription  ,   0 ,  0 },
};

// expression
edge expression[] = {
/*Num    type         ,            union               , action, next, alt */
/* 0*/  {edgeSymbol   , {(unsigned long)'-'}           , NULL  ,   2 ,  1 },
/* 1*/  {edgeNil      , {(unsigned long)0}             , NULL  ,   9 ,  0 },
/* 2*/  {edgeGraph    , {(unsigned long)term}          , ex1  ,   3 ,  0 },
/* 3*/  {edgeNil      , {(unsigned long)0}             , NULL  ,   4 ,  0 },
/* 4*/  {edgeSymbol   , {(unsigned long)'+'}           , NULL  ,   5 ,  6 },
/* 5*/  {edgeGraph    , {(unsigned long)term}          , ex2  ,   3 ,  0 },
/* 6*/  {edgeSymbol   , {(unsigned long)'-'}           , NULL  ,   7 ,  8 },
/* 7*/  {edgeGraph    , {(unsigned long)term}          , ex3  ,   3 ,  0 },
/* 8*/  {edgeGraphEnd , {(unsigned long)0}             , NULL  ,   0 ,  0 },

/* 9*/  {edgeGraph , {(unsigned long)term}          , NULL  ,   3 ,  0 },
};

// term
edge term[] = {
/*Num    type         ,            union               , action, next, alt */
/* 0*/  {edgeGraph    , {(unsigned long)factor}        , NULL  ,   1 ,  0 },
/* 1*/  {edgeNil      , {(unsigned long)0}             , NULL  ,   2 ,  0 },
/* 2*/  {edgeSymbol   , {(unsigned long)'*'}           , NULL  ,   3 ,  4 },
/* 3*/  {edgeGraph    , {(unsigned long)factor}        , te1  ,   1 ,  0 },
/* 4*/  {edgeSymbol   , {(unsigned long)'/'}           , NULL  ,   5 ,  6 },
/* 5*/  {edgeGraph    , {(unsigned long)factor}        , te2  ,   1 ,  0 },
/* 6*/  {edgeGraphEnd , {(unsigned long)0}             , NULL  ,   0 ,  0 },
};


// statement
edge statement[] = {
/*Num    type         ,            union               , action, next, alt */
/* 0*/  {edgeMorphem  , {(unsigned long)morphemIdent}  , st1  ,   1 ,  3 },
/* 1*/  {edgeSymbol   , {(unsigned long)130}/*:=*/     , NULL  ,   2 ,  0 },
/* 2*/  {edgeGraph    , {(unsigned long)expression}    , st2  ,   0 ,  0 },
/* 3*/  {edgeSymbol   , {(unsigned long)IF}            , NULL  ,   4 ,  7 },
/* 4*/  {edgeGraph    , {(unsigned long)condition}     , st3  ,   5 ,  0 },
/* 5*/  {edgeSymbol   , {(unsigned long)THEN}          , NULL  ,   6 ,  0 },
/* 6*/  {edgeGraph    , {(unsigned long)statement}     , st4  ,   0 ,  0 },
/* 7*/  {edgeSymbol   , {(unsigned long)WHILE}         , st5  ,   8 , 11 },
/* 8*/  {edgeGraph    , {(unsigned long)condition}     , st6  ,   9 ,  0 },
/* 9*/  {edgeSymbol   , {(unsigned long)DO}            , NULL  ,  10 ,  0 },
/*10*/  {edgeGraph    , {(unsigned long)statement}     , st7  ,   0 ,  0 },
/*11*/  {edgeSymbol   , {(unsigned long)BEGIN}         , NULL  ,  12 , 15 },
/*12*/  {edgeGraph    , {(unsigned long)statement}     , NULL  ,  13 ,  0 },
/*13*/  {edgeSymbol   , {(unsigned long)';'}           , NULL  ,  12 , 14 },
/*14*/  {edgeSymbol   , {(unsigned long)END}           , NULL  ,   0 ,  0 },
/*15*/  {edgeSymbol   , {(unsigned long)CALL}          , NULL  ,  16 , 17 },
/*16*/  {edgeMorphem  , {(unsigned long)morphemIdent}  , st8  ,   0 ,  0 },
/*17*/  {edgeSymbol   , {(unsigned long)'?'}           , NULL  ,  18 , 19 },
/*18*/  {edgeMorphem  , {(unsigned long)morphemIdent}  , st9  ,   0 ,  0 },
/*19*/  {edgeSymbol   , {(unsigned long)'!'}           , NULL  ,  20 , 21 },
/*20*/  {edgeGraph    , {(unsigned long)expression}    , st10  ,   0 ,  0 },
/*21*/  {edgeGraphEnd , {(unsigned long)0}             , NULL  ,   0 ,  0 },
};


// factor
edge factor[] = {
/*Num    type         ,            union               , action, next, alt */
/* 0*/  {edgeMorphem  , {(unsigned long)morphemNumeral}, fa1  ,   0 ,  1 },
/* 1*/  {edgeSymbol   , {(unsigned long)'('}           , NULL  ,   2 ,  4 },
/* 2*/  {edgeGraph    , {(unsigned long)expression}    , NULL  ,   3 ,  0 },
/* 3*/  {edgeNil      , {(unsigned long)0}             , NULL  ,   0 ,  0 },
/* 4*/  {edgeMorphem  , {(unsigned long)morphemIdent}  , fa2  ,   0 ,  0 },
};


// condition
edge condition[] = {
/*Num    type         ,            union               , action, next, alt */
/* 0*/  {edgeSymbol   , {(unsigned long)ODD}           , NULL  ,   1 ,  2 },
/* 1*/  {edgeGraph    , {(unsigned long)expression}    , co1  ,   0 ,  0 },
/* 2*/  {edgeGraph    , {(unsigned long)expression}    , NULL  ,   3 ,  0 },
/* 3*/  {edgeSymbol   , {(unsigned long)'='}           , co2  ,   9 ,  4 },
/* 4*/  {edgeSymbol   , {(unsigned long)'#'}           , co3  ,   9 ,  5 },
/* 5*/  {edgeSymbol   , {(unsigned long)'<'}           , co4  ,   9 ,  6 },
/* 6*/  {edgeSymbol   , {(unsigned long)'>'}           , co6  ,   9 ,  7 },
/* 7*/  {edgeSymbol   , {(unsigned long)128}/*<=*/     , co5  ,   9 ,  8 },
/* 8*/  {edgeSymbol   , {(unsigned long)129}/*>=*/     , co7  ,   9 ,  0 },
/* 9*/  {edgeGraph    , {(unsigned long)expression}    , co8  ,   0 ,  0 },
};


int parser(edge* graph);

int main (int argc, char* argv[])
{
    // Initialization
    lineCounter = 1;
    // declare global constArray
    constArray = (int32_t*) malloc(constArraySize * sizeof(long));

    // set procedureCounter to 0
    procedureCounter = 0;

    // init memory, codepointer, and codeLength
    codeMemoryRange = 1024;
    pCode = malloc(sizeof(char) * codeMemoryRange);
    codeStartAdress = pCode;

    // create base procedure
    namelistProcedure *pProcedure = createNamelistProcedure(NULL);
    currentProcedure = pProcedure;
    mainProcedure = pProcedure;

    // create labelList
    labelList = createLabellist();

    // is there an argument?
    if(argc < 2)
    {
        printf("Missing argument: PL0 file!\n");
        exit(-1);
    }

    // check on file type
    char fileType[4];
    strcpy(fileType, argv[1] + strlen(argv[1])-4);

    if(strlen(argv[1]) < 4 || strcmp(".pl0", fileType) != 0){
        printf("Wrong file type: '.pl0' file!\n");
        exit(-1);
    }

    // open file to read
    file = fopen(argv[1], "r");
    if(file == NULL)
    {
        printf("Error: main: input file could not be opened!\n");
        exit(EXIT_FAILURE);
    }

    // create new file name
    char outputFileName[strlen(argv[1])];
    strcpy(outputFileName, argv[1]);
    outputFileName[strlen(outputFileName)-1-2] = 'c';
        
    // open file to write
    outputFile = fopen(outputFileName, "w");
    if(outputFile == NULL)
    {
        printf("Error: main: compilerOutput could not be opened!\n");
        exit(EXIT_FAILURE);
    }
    // fill output file with 0s
    else{
        long x = 0L;
        fwrite(&x,sizeof(int32_t),1,outputFile);
    }
    
    printf("compile");

    // init lexer one time only
    read();

    // init morph with first token
    morph = lexer(file);

    // parse tokens
    int parserReturn = parser(programm);
    if(parserReturn != 1){
        printf("\n\nError: parser: Illegal pl0 statement, arround: %ld\n\n", morph.lineNumber);
    }
    else
        printf("\n\ndone\n");
    
    // printf("Parser result: %i\n", parserReturn);

    // close files
    fclose(file);

    // close output file ---
    int ret = -1;

    // write const block
    // set filePointer to the end
    fseek(outputFile, 0, SEEK_END);
    if(constArraySize > 0){
        ret = fwrite(constArray, sizeof(int32_t), constArraySize, outputFile);
    }


    // create buffer for procedure Index with size of 2 (little Endian)
    char bufProcedureIndex[2];

    fclose(outputFile);

    free(constArray);

    free(currentProcedure);
    
    return 0;
}

int parser(edge* graph)
{
    // function is executed for every single edge
    edge* currentGraph = graph;
    
    // init success variable
    int success = 0;

    while(1)
    {
        debugPrintParser("Next cycle\n");
        // switch case with different edge types
        switch(currentGraph->type){
            case edgeNil:
                    success = 1;
                    debugPrintParser("taking Nil edge\n");
                break;

            case edgeSymbol:
                    if(currentGraph->edgeValue.morphemID == morph.symbol){
                        success = 1;
                        debugPrintParser("Symbol found: accepted\n");
                     }else{
                        success = 0;
                        debugPrintParser("Symbol found: not accepted\n");
                     }
                break;

            case edgeMorphem:            
                    if(currentGraph->edgeValue.morphemID == morph.id){
                        success = 1;
                        debugPrintParser("Morphem found: accepted\n");
                     }else{
                        success = 0;
                        debugPrintParser("Morphem found: not accepted\n");
                     }
                break;

            case edgeGraph:
                    debugPrintParser("\n==========================\nNew Graph\n");
                    success = parser(currentGraph->edgeValue.graphAdress);
                    
                    if(debugParser)
                        printf("\nsuccess = %i\nleaving Graph\n--------------------------\n\n", success);
                break;

            case edgeGraphEnd:
                    return 1;
                break;
        }

        // find out what action to do
        if(success == 1 && currentGraph->functionPointer != NULL)
            success = currentGraph->functionPointer();
        
        // printf("success: %i\n", success);

        if(success == 0)
        {
            if(currentGraph->alternativeEdge == 0)
                return 0;

            // why " + pGraph"
            debugPrintParser("Alternative edge\n");
            currentGraph = graph + currentGraph->alternativeEdge;
        }
        else
        {
            if(currentGraph->type == edgeSymbol || currentGraph->type == edgeMorphem)
            {
                morph = lexer(file);
                debugPrintParser("get new token: ");
                // printf("Line: %ld\n", lineCounter);
        
                switch(morph.id)
                {
                    case 0:
                        if(debugParser)
                            printf("morph.word = %s\n", morph.word);
                        break;
                    case 1:
                        if(debugParser)
                            printf("morph.symbol = %i\n", morph.symbol);
                        break;
                    case 2:
                        if(debugParser)
                            printf("morph.number = %i\n", morph.number);
                        break;
                    default:
                        if(debugParser)
                            printf("ERROR!");
                }
            }

            if(currentGraph->nextEdge == 0)
                return success;

            debugPrintParser("\nNext edge\n");
            currentGraph = graph + currentGraph->nextEdge;
        }
        if(morph.id == -1)
            return success;
    }

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

/*
void debugPrintParser(char* message)
{
    if(debugParser > 0)
    {
        printf("%s", message);
    }
    return;
}
*/

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
                printf("morph.allSymbol = %i\n", morph.symbol);
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
    */
