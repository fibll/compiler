#include <stdio.h>
#include <stdlib.h>
#include "namelist.h"

// Basic List Functions ---
list *createList(void)
{
	list *pTmp;
	pTmp = malloc(sizeof(list));
	if(pTmp != NULL)
	{
		pTmp->first = pTmp->last = pTmp->current = NULL;
	}
	return pTmp;
}

int insertTail(list *pList, namelistNode *itemIns)
{
	pNode *pTmp = malloc(sizeof(pNode));
	pTmp->next = NULL;
	if(pTmp)
	{
		pTmp->item = itemIns;
		if(pList->first == NULL)
		{
			// Liste ist noch Leer
			pList->first = pList->last = pTmp;
		}
		else
		{
			// Liste enthält schon etwas
			pList->last->next = pTmp;
			pList->last = pTmp;	
		}
		pList->current = pTmp;
	}
	return (int)pTmp;
}

namelistNode *getFirst (list *pList)
{
	pNode *pTmp;
	pTmp = pList->first;
	if(pTmp)
	{
		pList->current = pTmp;
		return pTmp->item;	
	}
	else
	{
		return NULL;
	}
}

namelistNode *getNext(list *pList)
{
	pNode *pTmp;
	pTmp = pList->current;
	if(pTmp)
	{
		if(pTmp == pList->last)
		{
			return 0;
		}
		else
		{
			pList->current = pTmp->next;
			return pList->current->item;
		}
	}
}


// Namelist Functions ---
namelistNode *createNamelistNode(char *nodeName)
{
	// create space for new node with size of struct
	namelistNode *pNode = malloc(sizeof(namelistNode));
	if(pNode == NULL)
		return NULL;

	// initialize id (node), name (nodeName)
	pNode->id = node;
	pNode->pName = nodeName;

	// return node
	return pNode;
}

namelistConst *createNamelistConst(long value, long *constArray)
{
	// create space for new const with size of struct
	namelistConst *pConst = malloc(sizeof(namelistConst));
	if(pConst == NULL)
		return NULL;

	// initialize id (const), value
	pConst->id = constant;
	pConst->value = value;

	// save constant to dynamic constant array and initialize index
		// first item of the constArray is length
		// new item for constArray current length is on index 0
		// update length of array in item 0
		constArray[0]++;
		constArray = realloc(constArray, constArray[0] * sizeof(long));

		// add constant to the list
		constArray[constArray[0]] = value;

		// initialize const index with array index
		pConst->index = constArray[0];

	// return const
	return pConst;
	;
}

namelistConst *searchConst(long value, list *pList)
{
	// init first list node

	// loop: go through current list
		// check pObject to be an constant (id)
		// yes: 
			// compare value/name
			// correct:
				// return constant struct
		// init next list node
	// loop end
	;
}

namelistVariable *createNamelistVariable(namelistProcedure *pProcedure)
{
	// create space for new variable with size of struct
	namelistVariable *pVariable = malloc(sizeof(namelistVariable));
	if(pVariable == NULL)
		return NULL;

	// initialize id (var), offset
	pVariable->id = variable;
	pVariable->offset = pProcedure->variableCounter;

	// update pList.variableCounter
	pProcedure->variableCounter += 4;

	return pVariable;
}

namelistProcedure *createNamelistProcedure(namelistProcedure *pParentProcedure)
{
	// create space for new procedure with size of struct
	namelistProcedure *pProcedure = malloc(sizeof(namelistProcedure));
	
	/* initialize id (procedure), 
			procedure index (parentIndex + 1)
			parent,
			pList (new list), 
			variableCounter = 0
	*/
	pProcedure->id = procedure;
	
	if(pParentProcedure == NULL){
		pProcedure->procedureIndex = 0;
		pProcedure->pParentProcedure = NULL;
	}
	else{
		pProcedure->procedureIndex = pParentProcedure->procedureIndex + 1;
		pProcedure->pParentProcedure = pParentProcedure;
	}

	pProcedure->pList = createList();
	pProcedure->variableCounter = 0;

	// return procedure
	return	pProcedure;
}


namelistNode *searchNamelistNode(namelistProcedure pProcedure, char *nodeName)
{
	;
}

namelistNode *searchNamelistNodeGlobal(char *nodeName)
{
	;
}

// delete functions?
int deleteNamelistConst(namelistConst *pConst){
	free(pConst);
	return 0;
}

int deleteNamelistVariable(namelistVariable *pVariable){
	free(pVariable);
	return 0;
}

int deleteNamelistProcedure(namelistProcedure *pProcedure){
	return 0;
}

int deleteNamelistNode(namelistNode *pNode){
	free(pNode);
	return 0;
}

int deleteList_NOT_READY(list *pList){
	
	// set first item of list as current
	getFirst(pList);

	// go with for loog through list until current->next is NULL
	while(pList->current != NULL)
		// on current list item:
		// free var, const or proc
		/*
		switch(pList->current->item->pObject->id)
		{
			case 1:
				deleteNamelistProcedure(pList->current->item);
				break;
			case 2:
				deleteNamelistVariable(pList->current->item);
				break;
			case 3:
				deleteNamelistConst(pList->current->item);
				break;
			default:
				perror("Error on deleteList!");
				exit(-1);
		}
		*/

		// free item (namelistNode)
		free(pList->current->item);

		// set next node as current node of list
		pNode *pTmp = pList->current;
		pList->current = pList->current->next;

		// free node
		free(pTmp);

	// free list
	free(pList);

	return 0;
}

/*
bl1()



*/