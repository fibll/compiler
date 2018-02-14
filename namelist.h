#ifndef NAMELIST_H
#define NAMELIST_H

#include <stdlib.h>

// namelist item node (aka tBez)
typedef struct{
    int id;
    short prodecureIndex;
    void *pObject;
    int length;
    char *pName;
    size_t lineNumber;
}namelistNode;

// namelist item variable
typedef struct{
    int id;
    int offset;
}namelistVariable;

// namelist item constant
typedef struct{
    int id;
    int32_t value;
    int index;
}namelistConst;

// basic list node
typedef struct NODE
{
    struct NODE *next;
    namelistNode *item;	
}pNode;

// basic list
typedef struct LIST
{
    pNode *first;
    pNode *last;
    pNode *current;
}list;

// namelist item procedure
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

// namelist functions
namelistNode *createNamelistNode(char *nodeName, int inputId);
namelistConst *createNamelistConst(int32_t value);
namelistVariable *createNamelistVariable(namelistProcedure *pProcedure);
namelistProcedure *createNamelistProcedure(namelistProcedure *pParentProcedure);

namelistConst *searchConst(int32_t value, list *pList);
namelistNode *searchNamelistNode(namelistProcedure *pProcedure, char *nodeName);
namelistNode *searchNamelistNodeGlobal(namelistProcedure *pProcedure, char *nodeName);

// delete functions
int deleteNamelistNode(namelistNode *pNode);
int deleteList(list *pList);

#endif