// morphem
typedef struct morph
{
    int id;
    char word[30];
    char symbol[2];
    int number;
    char keyword[30];
}MORPHEM;

// edgeType
typedef enum EDGE_TYPE
{
    edgeNil = 0,  // NIL     
    edgeSymbol = 1,  // Symbol  
    edgeMorphem = 2,  // Morphem 
    edgeGraph = 4,  // Graph   
    edgeGraphEnd = 8,  // Graphende 
}edgeType;

// edges
typedef struct EDGE
{
    edgeType type;  
    
    union EDGE_DESCRIPTION 
    {
        unsigned long unionLengthVar;       // guarantees that the union save space is long enough 
        int           symbol;               // symbol ( '(', ')' )
        int           morphemID;            // Morphem id (number, word, etc)
        void *        graphAdress;          // pointer to following Graph 
    }   edgeDescription;

    int (*funktionPointer)(void); 
    int nextEdge; 
    int alternativeEdge; 
}edge;

enum
{
    morphemIdent, 
    morphemNumeral
};
