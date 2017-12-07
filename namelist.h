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

// namelist node
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
}namelistVariable;

// namelist constant
typedef struct{
    int id;
    long value;
    int index;
}namelistConst;

// namelist procedure
typedef struct NAME_LIST_PROCEDURE{
    int id;
    short procedureIndex;
    struct NAME_LIST_PROCEDURE *pParentProcedure;
    list *pList;
    int variableCounter;
}namelistProcedure;
