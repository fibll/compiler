#include <stdio.h>
#include <stdlib.h>
#include "namelist.h"
#include "types.h"

static MORPHEM morph;
static int constArraySize;
static long *constArray;
static namelistProcedure *currentProcedure;

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
	else{
		return -1;
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
	return NULL;
}

// Namelist Functions ---
namelistNode *createNamelistNode(char *nodeName, int inputId)
{
	// create space for new node with size of struct
	namelistNode *pNode = malloc(sizeof(namelistNode));
	if(pNode == NULL)
		return NULL;

	// initialize id (input id: what it really is), name (nodeName)
	pNode->id = inputId;
	pNode->pName = nodeName;

	// return node
	return pNode;
}

namelistConst *createNamelistConst(long value)
{
	// create space for new const with size of struct
	namelistConst *pConst = malloc(sizeof(namelistConst));
	if(pConst == NULL)
		return NULL;

	// initialize id (const), value
	pConst->id = constant;
	pConst->value = value;

	// search in constArray for an constant with the same value?
	int i = 0;
	for(i = 0; i < constArraySize; i++)
	{
		// if there is one:
		if(constArray[i] == value)
		{
			// take index from existing constant and write it into const.index
			// initialize const index with array index
			pConst->index = i;

			// return const
			return pConst;
		}
	}

	// if there is no constant with the same value:
	// save constant to dynamic constant array and initialize index
	// first item of the constArray is length
	// new item for constArray current length is on index 0
	// update length of array in item 0
	constArraySize++;
	long* tmpArray = (long* )realloc(constArray, constArraySize * sizeof(long));

	if(tmpArray == NULL){
		printf("\n\n\nfailure\n\n\n");
	}
	else{
		constArray = tmpArray;
	}

	// add constant to the array
	constArray[constArraySize-1] = value;

	//printf("test 2\n");
	// initialize const index with array index
	pConst->index = constArraySize;

	// return const
	return pConst;
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

namelistConst *searchConst(long value, list *pList)
{
	// init first list node
	if(getFirst(pList) == NULL)
		return NULL;

	// loop: go through current list
	do {
		// check pObject to be an constant (id)
		// yes:
		if(pList->current->item->id == constant){	
				namelistConst *tmpConst = pList->current->item->pObject;
				// compare value/name
				// correct:
				if(value == tmpConst->value){
					// return constant struct
					return tmpConst;
				}
		}
		// init next list node
		getNext(pList);

	} while(pList->current != pList->last);

	return NULL;
}

namelistNode *searchNamelistNode(namelistProcedure *pProcedure, char *nodeName)
{
	// set namelist pointer to first list item of given procedure namelist
	getFirst(pProcedure->pList);
	if(pProcedure->pList->current->item->pName == nodeName)
		return pProcedure->pList->current->item;

	// go through list with loop until your through the list...:
	while(getNext(pProcedure->pList)){		
		// check if nodeName is name of current item (namelist node)
		if(pProcedure->pList->current->item->pName == nodeName)
			return pProcedure->pList->current->item;
	}
	
	// return not found
	return NULL;
}


namelistNode *searchNamelistNodeGlobal(namelistProcedure *pProcedure, char *nodeName)
{
	// set currentProcedure to input procedure
	namelistProcedure *currentProcedure = pProcedure;
	namelistNode *searchResult = NULL;

	// search with loop until search function until parentProcedure is NULL (do while):
	do{
		// search in currentProcedure
		searchResult = searchNamelistNode(currentProcedure, nodeName);

		// if return of search is not null:
		if(searchResult != NULL)
			return searchResult;

		// set currentProcedure to parentProcedure	
		currentProcedure = currentProcedure->pParentProcedure;

	}while(currentProcedure != NULL);
	
	// node not found
	return NULL;
}

// delete functions --
int deleteNamelistNode(namelistNode *pNode){

	if(pNode->id == 1){
		// delete namelist in Procedure
		namelistProcedure *pProcedure = pNode->pObject;

		// FIX: check if the pProcedure->pList is already free;
		deleteList(pProcedure->pList);
	}
	else {
		free(pNode->pObject);
	}
	free(pNode);
	return 0;
}

int deleteList(list *pList){
	
	// set first item of list as current
	getFirst(pList);

	// go with for loop through list until current->next is NULL
	while(pList->current != NULL)
	{
		// free item (namelistNode)
		deleteNamelistNode(pList->current->item);

		// set next node as current node of list
		pNode *pTmp = pList->current;
		pList->current = pList->current->next;

		// free node
		free(pTmp);
	}
	// free list
	free(pList);

	return 0;
}

// bl1
int blockAcceptConstantIdentifier() {
	int ret;
	char *nodeName = morph.word;

	// search for the nodeName
	namelistNode *tmpNode = searchNamelistNode(currentProcedure, nodeName);

	if(tmpNode != NULL){
		// return failure
		printf("Error: Identifier exists already");
		return -1;
	}
	else {
		// create new node with name
		tmpNode = createNamelistNode(nodeName, constant);

		// insert the new node into the list of the current procedure list
		ret = insertTail(currentProcedure->pList, tmpNode);
		if(ret == -1){
			printf("Error: Insert did not work!\n");
			return -2;
		}
	}
	return 0;
}

// bl2
int blockAcceptConstantValue() {
	long value = morph.number;

	// write pointer of constNode into node
	currentProcedure->pList->current->item->pObject = createNamelistConst(value);
	if(currentProcedure->pList->current->item->pObject == NULL){
		printf("Error: Constant couldn't be created!\n");
		return -1;
	}
}

// bl3
int blockAcceptVariable() {
	int ret;
	char *nodeName = morph.word;

	// search for nodeName
	namelistNode *tmpNode = searchNamelistNode(currentProcedure, nodeName);
	
	if(tmpNode != NULL){
		// return failure
		printf("Error: Identifier exists already");
		return -1;
	}
	else{
		// create node with nodeName
		ret = insertTail(currentProcedure->pList, tmpNode);
		if(ret == -1){
			printf("Error: Insert did not work!\n");
			return -2;
		}
	}

	// write pointer of varNode into node
	currentProcedure->pList->current->item->pObject = createNamelistVariable(currentProcedure);
	if(currentProcedure->pList->current->item->pObject == NULL){
		printf("Error: Variable couldn't be created!\n");
		return -3;
	}
}

// bl4
int blockAcceptProcedure() {
	int ret;
	char *nodeName = morph.word;

	// search for nodeName
	namelistNode *tmpNode = searchNamelistNode(currentProcedure, nodeName);
	
	if(tmpNode != NULL){
		// return failure
		printf("Error: Identifier exists already");
		return -1;
	}
	else{
		// create node with nodeName
		ret = insertTail(currentProcedure->pList, tmpNode);
		if(ret == -1){
			printf("Error: Insert did not work!\n");
			return -2;
		}
	}

	// write pointer of procedureNode into node
	currentProcedure->pList->current->item->pObject = createNamelistProcedure(currentProcedure);
	if(currentProcedure->pList->current->item->pObject == NULL){
		printf("Error: Procedure couldn't be created!\n");
		return -3;
	}
	
	// new procedure is now current procedure
	currentProcedure = currentProcedure->pList->current->item->pObject;
}

// bl5
int blockEndOfProcedureDescription(void) {
	/*
		Codegenerierung
		siehe V5_NameList.pdf
	*/

	// delete list of current procedure
	deleteList(currentProcedure->pList);

	// parentProcedure is now current procedure
	if(currentProcedure->pParentProcedure != NULL){
		currentProcedure = currentProcedure->pParentProcedure;
	}
	else {
		printf("Reached top procedure!\n");
		exit(0);
	}
}