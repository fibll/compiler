#ifndef PARSER_H
#define PARSER_H

// edgeType
typedef enum EDGE_TYPE
{
    edgeNil = 0,  // NIL     
    edgeSymbol = 1,  // Symbol  
    edgeMorphem = 2,  // Morphem 
    edgeGraph = 3,  // Graph   
    edgeGraphEnd = 4,  // Graphende 
}edgeType;

// edges
typedef struct EDGE
{
    edgeType type;  
    
    union EDGE_VALUE 
    {
        unsigned long unionLengthVar;       // guarantees that the union save space is long enough 
        int           symbol;               // symbol ( '(', ')' )
        int           morphemID;            // Morphem id (number, word, etc)
        void         *graphAdress;          // pointer to following Graph 
    }   edgeValue;

    int (*functionPointer)(void); 
    int nextEdge; 
    int alternativeEdge; 
}edge;

int parser(edge* graph);
void debugPrintParser(char* message);

#endif