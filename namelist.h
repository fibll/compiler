#ifndef NAMELIST_H
#define NAMELIST_H

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

// labels for codegeneration
typedef struct tLABEL
{
    int id;
    long iJmp;
}tLabel;


// Basic list types ---
// namelist node
typedef struct NODE
{
    struct NODE *next;
    namelistNode *item;	
}pNode;

// namelist
typedef struct
{
    pNode *first;
    pNode *last;
    pNode *current;
}list;

// label list ---
typedef struct LABELNODE
{
    struct LABELNODE *next;
    tLabel *item;	
}pLabelNode;

// namelist
typedef struct
{
    pLabelNode *first;
    pLabelNode *last;
    pLabelNode *current;
}labellist;
// End Basic list types ---


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
  constant = 3,
  label = 4,
};

// namelist
list *createList(void);
int insertTail(list *pList, namelistNode *itemIns);
namelistNode *getFirst (list *pList);
namelistNode *getNext(list *pList);

// labellist
labellist *createLabellist(void);
int pushTailLabel(labellist *pList, tLabel *itemIns);
tLabel *popHeadLabel(labellist *pList);
tLabel *getFirstLabel(labellist *pList);
tLabel *getNextLabel(labellist *pList);

// namelist functions
namelistNode *createNamelistNode(char *nodeName, int inputId);
namelistConst *createNamelistConst(long value);
namelistVariable *createNamelistVariable(namelistProcedure *pProcedure);
namelistProcedure *createNamelistProcedure(namelistProcedure *pParentProcedure);

namelistConst *searchConst(long value, list *pList);
namelistNode *searchNamelistNode(namelistProcedure *pProcedure, char *nodeName);
namelistNode *searchNamelistNodeGlobal(namelistProcedure *pProcedure, char *nodeName);

// delete functions
int deleteNamelistNode(namelistNode *pNode);
int deleteList(list *pList);

// block functions
int blockAcceptConstantIdentifier(void); // aus morphem holen
int blockAcceptConstantValue(void); // aus morphem holen
int blockAcceptVariable(void); // aus morphem holen
int blockAcceptProcedure(void); // aus morphem holen
int blockEndOfProcedureDescription(void);

#endif