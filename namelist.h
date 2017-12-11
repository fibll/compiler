// namelist node
typedef struct NODE
	{
		struct NODE *next;
		void *item;	
	}pNode;

// namelist
typedef struct
	{
		pNode *first;
		pNode *last;
		pNode *current;
	}list;

// namelist node aka tBez
typedef struct{
    int id;
    short prodecureIndex;
    void *pObject;
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


// id enum
enum {
  node = 0,
  procedure = 1,
  variable = 2,
  constant = 3
};