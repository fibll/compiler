// lexer types ---
// morphem
typedef struct morph
{
    int id;         /*-1: empty, 0: word, 1: symbol, 2: number, 3: keyword, */
    char word[30];
    int symbol;
    int number;
}MORPHEM;

// symbols enum
/* 128 = "<="
 * 129 = ">="  
 * 130 = ":="
 * 131 = "BEGIN" 
 * 132 = ""
 * 133 = "DO"
 * 134 = "END"
 * 135 = "IF"
 * 136 = "ODD"
 * 137 = "PROCEDURE"
 * 138 = "THEN"
 * 139 = "VAR"
 * 140 = "WHILE"
 * 141 = "CALL"
 * 142 = "CONST"
*/

typedef enum SYMBOLS
{
    BEGIN = 131,
    DO = 133,
    END = 134,
    IF = 135,
    ODD = 136,
    PROCEDURE = 137,
    THEN = 138,
    VAR = 139,
    WHILE = 140,
    CALL = 141,
    CONST = 142
}symbols;

// graph types ---
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

enum
{
    morphemIdent = 0, 
    morphemNumeral = 2,
};

// namelist types ---

// namelist node
typedef struct NODE
	{
		struct NODE *next;
		void *item;	
	}node;

// namelist
typedef struct
	{
		node *first;
		node *last;
		node *current;
	}list;

// namelist item
typedef struct{
    int id;
    short prodecureIndex;
    void *pObjekt;
    int length;
    char *pName;
}namelistNode;

// namelist variable
typedef struct{
    int id;
    int offset;
}nameListVariable;

// namelist constant
typedef struct{
    int id;
    long value;
    int index;
}nameListConstant;

// namelist procedure
typedef struct NAME_LIST_PROCEDURE{
    int id;
    short procedureIndex;
    struct NAME_LIST_PROCEDURE;
    list *pList;
    int variableCounter;
}nameListProcedure;