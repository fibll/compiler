#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "namelist.h"
#include "types.h"

static MORPHEM morph;
static int constArraySize;
static long *constArray;
static namelistProcedure *currentProcedure;
static short codeLength;
static labellist *labelList;

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

/*
int insertHead(list *pList, namelistNode *itemIns)
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
			pTmp->next = pList->first;
			pList->first = pTmp;
		}
		pList->current = pTmp;
	}
	else{
		return -1;
	}
	return (int)pTmp;
}
*/

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


// label list Functions ---
// Basic List Functions ---
labellist *createLabellist(void)
{
	labellist *pTmp;
	pTmp = malloc(sizeof(list));
	if(pTmp != NULL)
	{
		pTmp->first = pTmp->last = pTmp->current = NULL;
	}
	return pTmp;
}

int pushHeadLabel(labellist *pList, tLabel *itemIns)
{
	pLabelNode *pTmp = malloc(sizeof(pLabelNode));
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
			pTmp->next = pList->first;
			pList->first = pTmp;
		}
		pList->current = pTmp;
	}
	else{
		return -1;
	}
	return (int)pTmp;
}

tLabel *popHeadLabel(labellist *pList)
{
	// if list is null
	if(pList == NULL || pList->first == NULL)
		return NULL;

	// init pTmp with first
	pLabelNode *pTmp = pList->first;

	// change list pointer
	// if list got second
	if(pList->first->next == NULL){
		pList->first = NULL;
		pList->current = NULL;
		pList->last = NULL;
	}
	// else set list->first to second
	else
		pList->first = pList->first->next;
	
	// return pTmp
	tLabel *tmpLabel = pTmp->item;
	free(pTmp);
	return tmpLabel;
}

tLabel *getFirstLabel(labellist *pList)
{
	pLabelNode *pTmp;
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

tLabel *getNextLabel(labellist *pList)
{
	pLabelNode *pTmp;
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
	pNode->pName = malloc(sizeof(char) * strlen(nodeName));
	strcpy(pNode->pName, nodeName);

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
	pConst->index = constArraySize-1;

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
	if(pProcedure->pList->current == NULL)
		return NULL;
	
	if(strcmp(pProcedure->pList->current->item->pName, nodeName) == 0)
		return pProcedure->pList->current->item;

	// go through list with loop until your through the list...:
	while(getNext(pProcedure->pList)){	

		// check if nodeName is name of current item (namelist node)
		if(strcmp(pProcedure->pList->current->item->pName, nodeName) == 0)
			return pProcedure->pList->current->item;

	}
	
	// return not found
	return NULL;
}


namelistNode *searchNamelistNodeGlobal(namelistProcedure *pProcedure, char *nodeName)
{
	// save currentProcedure, so you can restore it later again
	namelistProcedure * oldCurrentProcedure = currentProcedure;

	// set input procedure as currentProcedure
	currentProcedure = pProcedure;
	namelistNode *searchResult = NULL;

	// search with loop until search function until parentProcedure is NULL (do while):
	do{
		// search in currentProcedure
		searchResult = searchNamelistNode(currentProcedure, nodeName);

		// if return of search is not null:
		if(searchResult != NULL){
			// restore old currentProcedure
			currentProcedure = oldCurrentProcedure;
			return searchResult;
		}

		// set currentProcedure to parentProcedure	
		currentProcedure = currentProcedure->pParentProcedure;

	}while(currentProcedure != NULL);
	
	// restore old currentProcedure
	currentProcedure = oldCurrentProcedure;
	
	// node not found
	return NULL;
}

// label function --
int pushLabel(labellist *listOfLabels, int id, long iJmp){
	tLabel *pTmp = malloc(sizeof(tLabel));
	pTmp->id = id;
	pTmp->iJmp = iJmp;

	return pushHeadLabel(listOfLabels, pTmp);
}

long popLabel(labellist *listOfLabels){
	tLabel *pTmp = popHeadLabel(listOfLabels);
	long iJump = pTmp->iJmp;
	free(pTmp);

	return iJump;
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

	// cause success should be 1 if everything worked good
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
	//free(pList);

	return 0;
}

// block functions ========================================
// bl1
int blockAcceptConstantIdentifier() {
	printf("blockAcceptConstantIdentifier\n");
	int ret;
	char nodeName[strlen(morph.word)];
	strcpy(nodeName, morph.word);

	// search for the nodeName
	namelistNode *tmpNode = searchNamelistNode(currentProcedure, nodeName);

	if(tmpNode != NULL){
		// return failure
		printf("Error: Const:Identifier exists already\n");
		exit(EXIT_FAILURE);
	}
	else {
		// create new node with name
		tmpNode = createNamelistNode(nodeName, constant);

		// insert the new node into the list of the current procedure list
		ret = insertTail(currentProcedure->pList, tmpNode);
		if(ret == -1){
			printf("Error: Insert did not work!\n");
			exit(EXIT_FAILURE);
		}
	}
	
	// cause success should be 1 if everything worked good
	return 1;
}

// bl2
int blockAcceptConstantValue() {
	printf("blockAcceptConstantValue\n");
	long value = morph.number;

	// write pointer of constNode into node
	currentProcedure->pList->current->item->pObject = createNamelistConst(value);
	if(currentProcedure->pList->current->item->pObject == NULL){
		printf("Error: Constant couldn't be created!\n");
		exit(EXIT_FAILURE);
	}

	// cause success should be 1 if everything worked good
	return 1;
}

// bl3
int blockAcceptVariable() {
	
	printf("blockAcceptVariable----------\n");
	int ret;
	char nodeName[strlen(morph.word)];
	strcpy(nodeName, morph.word);

	// test
	printf("Ident: %s\n", nodeName);

	// search for nodeName
	namelistNode *tmpNode = searchNamelistNode(currentProcedure, nodeName);

	if(tmpNode != NULL){
		// return failure
		printf("Error: Var:Identifier exists already\n");
		exit(EXIT_FAILURE);
	}
	else{
		// create node with nodeName
		tmpNode = createNamelistNode(nodeName, variable);

		// add tmpNode to list
		ret = insertTail(currentProcedure->pList, tmpNode);
		if(ret == -1){
			printf("Error: Insert did not work!\n");
			exit(EXIT_FAILURE);
		}
	}

	// write pointer of varNode into node
	currentProcedure->pList->current->item->pObject = createNamelistVariable(currentProcedure);
	if(currentProcedure->pList->current->item->pObject == NULL){
		printf("Error: Variable couldn't be created!\n");
		exit(EXIT_FAILURE);
	}

	// cause success should be 1 if everything worked good
	return 1;
}

// bl4
int blockAcceptProcedure() {
	printf("blockAcceptProcedure\n");
	int ret;
	char nodeName[strlen(morph.word)];
	strcpy(nodeName, morph.word);

	// search for nodeName
	namelistNode *tmpNode = searchNamelistNode(currentProcedure, nodeName);
	
	if(tmpNode != NULL){
		// return failure
		printf("Error: Proc:Identifier exists already\n");
		exit(EXIT_FAILURE);
	}
	else{
		// create node with nodeName
		tmpNode = createNamelistNode(nodeName, procedure);

		// add tmpNode to list
		ret = insertTail(currentProcedure->pList, tmpNode);
		if(ret == -1){
			printf("Error: Insert did not work!\n");
			exit(EXIT_FAILURE);
		}
	}

	// write pointer of procedureNode into node
	currentProcedure->pList->current->item->pObject = createNamelistProcedure(currentProcedure);
	if(currentProcedure->pList->current->item->pObject == NULL){
		printf("Error: Procedure couldn't be created!\n");
		exit(EXIT_FAILURE);
	}
	
	// new procedure is now current procedure
	currentProcedure = currentProcedure->pList->current->item->pObject;

	// cause success should be 1 if everything worked good
	return 1;
}

// bl5
int blockEndOfProcedureDescription(void) {
	printf("blockEndOfProcedureDescription\n");
	/*
		Codegenerierung:
		// retProc
		// renew the codeLength
		// codeLength = new codeLength (cause in the beginning you don't know how long the proc is gonna be)
	*/

	// delete list of current procedure
	deleteList(currentProcedure->pList);

	// parentProcedure is now current procedure
	if(currentProcedure->pParentProcedure != NULL){
		currentProcedure = currentProcedure->pParentProcedure;
	}
	else {
		printf("Error: Already at top procedure!\n");
	}

	// cause success should be 1 if everything worked good
	return 1;
}

// bl6
int startStatement(void){
	printf("startStatement\n");
	// initialize write code buffer
	// code generation:
	// entryProc(codeLen, indexProc, varLength)
	// => entryProc(codeLength (with 0 cause you will update it later), currentProcedure->Index, currentProcedure->variableCounter)

	// für codeLength muss noch Lösung gefunden werden, am besten trägt man die relative Adresse ein, 
	// so dass man später einfach über diese wieder darauf zugreifen kann um nach dem durchlaufen der
	// Procedur beschreibung die wirkliche Länge einzutragen.

	return 1;
}

// statement functions ========================================
// st1
int st1(void){
	printf("st1\n");

	int ret = 0;
	char nodeName[strlen(morph.word)];
	strcpy(nodeName, morph.word);

	// search name local
	namelistNode *tmpNode = searchNamelistNodeGlobal(currentProcedure, nodeName);

	// not found
	if(tmpNode == NULL){
		// Error handling
		printf("Error: Identifier does not exist global!\n");
		exit(EXIT_FAILURE);
	}
	
	// node is variable?
	if(tmpNode->id != variable){
		// Error handling
		printf("Error: Identifier is not of type variable!\n");
		exit(EXIT_FAILURE);
	}
	
	// code generation:
		// PushAdrVarLocal
		// PushAdrVarMain
		// PushAdrVarGlobal
	return 1;
}

int st2(void){
	printf("st2\n");
	// code generation: store value

	return 1;
}

// if nach condition
int st3(void){
	printf("st3\n");

	// create label				  iJmp is index of jmp command
	// pushLabel(labelList, label, iJmp);

	// code generation: jnot mit vorläufiger Relativadresse 0

	return 1;
}

// if nach statement
int st4(void){
	printf("st4\n");

	//long tmp = popLabel(labelList);
	
	// calculate relative adress

	return 1;
}

// while
int st5(void){
	printf("st5\n");

	//pushLabel(labelList, 0, iJmp);
	
	// calculate relative adress

	return 1;
}

// while nach condition
int st6(void){
	printf("st6\n");
	//pushLabel(labelList, 0, iJmp);
	
	// code generation: jnot mit vorläufiger Relativadresse 0

	return 1;
}


// while nach statement
int st7(void){
	printf("st7\n");
	//popLabel(labelList);

	// calculate relative adress + 3 extra bytes for the jmp-command

	// pop second label
	//popLabel(labelList);

	// calculate relative adress, so that jmp after the first command gets to condition

	return 1;
}

// while nach statement
int st8(void){
	printf("st8\n");
	int ret = 0;
	char nodeName[strlen(morph.word)];
	strcpy(nodeName, morph.word);

	// search name local
	namelistNode *tmpNode = searchNamelistNodeGlobal(currentProcedure, nodeName);

	// not found
	if(tmpNode == NULL){
		// Error handling
		printf("Error: Identifier does not exist global!\n");
		exit(EXIT_FAILURE);
	}
	
	// node is variable?
	if(tmpNode->id != procedure){
		// Error handling
		printf("Error: Identifier is not of type procedure!\n");
		exit(EXIT_FAILURE);

		// return
		return -1;
	}

	// codegeneration: call procedurenumber

	return 1;
}


int st9(void) {
	printf("st9\n");
	int ret = 0;
	char nodeName[strlen(morph.word)];
	strcpy(nodeName, morph.word);

	// search name local
	namelistNode *tmpNode = searchNamelistNodeGlobal(currentProcedure, nodeName);

	// not found
	if(tmpNode == NULL){
		// Error handling
		printf("Error: Identifier does not exist global!\n");
		exit(EXIT_FAILURE);
	}
	
	// node is variable?
	if(tmpNode->id != variable){
		// Error handling
		printf("Error: Identifier is not of type variable!\n");
		exit(EXIT_FAILURE);
	}
	
	// code generation:
		// PushAdrVarLocal
		// PushAdrVarMain
		// PushAdrVarGlobal
	
		// getVal

	return 1;
}

int st10(void){
	printf("st10\n");
	// code generation: putVal

	return 1;
}

// expression functions ========================================
int ex1(void){
	printf("ex1\n");
	// code generation: vzMinus

	return 1;
}

int ex2(void){
	printf("ex2\n");
	// code generation: opAdd

	return 1;
}

int ex3(void){
	printf("ex3\n");
	// code generation: opSub

	return 1;
}

// term functions ========================================
int te1(void){
	printf("te1\n");
	// code generation: opMul

	return 1;
}

int te2(void){
	printf("te2\n");
	// code generation: opDiv

	return 1;
}

// factor functions ========================================
// Numberal
int fa1(void){
	printf("fa1\n");
	int ret = 0;
	int i = 0;
	long value = morph.number;

	// search const
	for(i = 0; i < constArraySize; i++){
		if(constArray[i] == value){
			ret = 1;
			break;
		}
	}

	// not found
	if(ret != 1){
		// insert constant (just in array)
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
	}

	// code generation: putConst(ConstIndex)

	return 1;
}

int fa2(void){
	printf("fa2\n");
	// Ident

	int ret = 0;
	char nodeName[strlen(morph.word)];
	strcpy(nodeName, morph.word);

	// search name local
	namelistNode *tmpNode = searchNamelistNodeGlobal(currentProcedure, nodeName);

	// not found
	if(tmpNode == NULL){
		// Error handling
		printf("Error: Identifier does not exist global!\n");
		exit(EXIT_FAILURE);
	}
	
	// node is variable or constant?	
	if(tmpNode->id != variable && tmpNode->id != constant){
		// Error handling
		printf("Error: Identifier is not of type variable or constant!\n");
		exit(EXIT_FAILURE);
	}

	// code generation:
	// if(tmpNode->id == variable)
		// puValVrLocl(displ)
		// puValVrMain(displ)
		// puValVrGlob(displ, ProcedureNr)
	
	// if(tmpNode->id == constant)
		// puConst(index)

	return 1;
}


// condition functions ========================================
int co1(void){
	printf("co1\n");

	// code generation: odd

	return 1;
}

int co2(void){
	printf("co2\n");

	// code generation: save compare operator '='

	return 1;
}

int co3(void){
	printf("co3\n");

	// save compare operator '#'

	return 1;
}

int co4(void){
	printf("co4\n");

	// save compare operator '<'

	return 1;
}

int co5(void){
	printf("co5\n");

	// save compare operator '<='

	return 1;
}

int co6(void){
	printf("co6\n");

	// save compare operator '>'

	return 1;
}

int co7(void){
	printf("co7\n");

	// save compare operator '>='

	return 1;
}

int co8(void){
	printf("co8\n");

	// code generation: compare operator (before saved)

	return 1;
}