#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "namelist.h"
#include "types.h"
#include "codeGen.h"

static int debugSMR = 0;

static MORPHEM morph;
static int constArraySize;
static int32_t *constArray;
static namelistProcedure *currentProcedure;
static short codeLength;
static labellist *labelList;
static int compareCase = 0;
static char* pCode;
static char* codeStartAdress;

static namelistProcedure *mainProcedure;


void debugPrintSMR(char* message){
    if(debugSMR > 0)
    {
        printf("%s", message);
    }
    return;
}

// block functions ========================================
// bl1
int blockAcceptConstantIdentifier() {
	debugPrintSMR("blockAcceptConstantIdentifier\n");
	int ret;
	char nodeName[strlen(morph.word)];
	strcpy(nodeName, morph.word);

	// search for the nodeName
	namelistNode *tmpNode = searchNamelistNode(currentProcedure, nodeName);

	if(tmpNode != NULL){
		// return failure
		printf("\n\nError: bl1: Identifier exists already\n");
        printf("Somewhere around line: %ld\n", morph.lineNumber);
		exit(EXIT_FAILURE);
	}
	else {
		// create new node with name
		tmpNode = createNamelistNode(nodeName, constant);

		// insert the new node into the list of the current procedure list
		ret = insertTail(currentProcedure->pList, tmpNode);
		if(ret == -1){
			printf("\n\nError: bl1: Insert did not work!\n");
            printf("Somewhere around line: %ld\n", morph.lineNumber);
			exit(EXIT_FAILURE);
		}
	}
	
	// cause success should be 1 if everything worked good
	return 1;
}

// bl2
int blockAcceptConstantValue() {
	debugPrintSMR("blockAcceptConstantValue\n");
	long value = morph.number;

	// write pointer of constNode into node
	currentProcedure->pList->current->item->pObject = createNamelistConst(value);
	if(currentProcedure->pList->current->item->pObject == NULL){
		printf("\n\nError: bl2: Constant couldn't be created!\n");
        printf("Somewhere around line: %ld\n", morph.lineNumber);
		exit(EXIT_FAILURE);
	}

	// cause success should be 1 if everything worked good
	return 1;
}

// bl3
int blockAcceptVariable() {
	
	debugPrintSMR("blockAcceptVariable----------\n");
	int ret;
	char nodeName[strlen(morph.word)];
	strcpy(nodeName, morph.word);

	// search for nodeName
	namelistNode *tmpNode = searchNamelistNode(currentProcedure, nodeName);

	if(tmpNode != NULL){
		// return failure
		printf("\n\nError: bl3: Var:Identifier exists already\n");
        printf("Somewhere around line: %ld\n", morph.lineNumber);
		exit(EXIT_FAILURE);
	}
	else{
		// create node with nodeName
		tmpNode = createNamelistNode(nodeName, variable);

		// add tmpNode to list
		ret = insertTail(currentProcedure->pList, tmpNode);
		if(ret == -1){
			printf("\n\nError: bl3: Insert did not work!\n");
            printf("Somewhere around line: %ld\n", morph.lineNumber);
			exit(EXIT_FAILURE);
		}
	}

	// write pointer of varNode into node
	currentProcedure->pList->current->item->pObject = createNamelistVariable(currentProcedure);
	if(currentProcedure->pList->current->item->pObject == NULL){
		printf("\n\nError: bl3: Variable couldn't be created!\n");
        printf("Somewhere around line: %ld\n", morph.lineNumber);
		exit(EXIT_FAILURE);
	}

	// cause success should be 1 if everything worked good
	return 1;
}

// bl4
int blockAcceptProcedure() {
	debugPrintSMR("blockAcceptProcedure\n");
	int ret;
	char nodeName[strlen(morph.word)];
	strcpy(nodeName, morph.word);

	// search for nodeName
	namelistNode *tmpNode = searchNamelistNode(currentProcedure, nodeName);
	
	if(tmpNode != NULL){
		// return failure
		printf("\n\nError: bl4: Identifier exists already\n");
        printf("Somewhere around line: %ld\n", morph.lineNumber);
		exit(EXIT_FAILURE);
	}
	else{
		// create node with nodeName
		tmpNode = createNamelistNode(nodeName, procedure);

		// add tmpNode to list
		ret = insertTail(currentProcedure->pList, tmpNode);
		if(ret == -1){
			printf("\n\nError: bl4: Insert did not work!\n");
			exit(EXIT_FAILURE);
		}
	}

	// write pointer of procedureNode into node
	currentProcedure->pList->current->item->pObject = createNamelistProcedure(currentProcedure);
	if(currentProcedure->pList->current->item->pObject == NULL){
		printf("\n\nError: bl4: Procedure couldn't be created!\n");
        printf("Somewhere around line: %ld\n", morph.lineNumber);
		exit(EXIT_FAILURE);
	}
	
	// new procedure is now current procedure
	currentProcedure = currentProcedure->pList->current->item->pObject;   

	// cause success should be 1 if everything worked good
	return 1;
}

// bl5
int blockEndOfProcedureDescription(void) {
	debugPrintSMR("blockEndOfProcedureDescription\n");
	
	// Codegenerierung:
	code(retProc);
	writeCodeToFile();

	// delete list of current procedure
	deleteList(currentProcedure->pList);

	// parentProcedure is now current procedure
	if(currentProcedure->pParentProcedure != NULL){
		currentProcedure = currentProcedure->pParentProcedure;
	}
	else {
		//printf("Warning: Already at top procedure!\n");
	}

	// cause success should be 1 if everything worked good
	return 1;
}

// bl6
int blockstartStatement(void){
	debugPrintSMR("startStatement\n");

	// initialize write code buffer
	// code generation:
	code(entryProc, 0, currentProcedure->procedureIndex, currentProcedure->variableCounter);
	
    return 1;
}

// statement functions ========================================
// st1
int st1(void){
	debugPrintSMR("st1\n");

	int ret = 0;
	char nodeName[strlen(morph.word)];
	strcpy(nodeName, morph.word);

	// search name local
	namelistNode *tmpNode = searchNamelistNodeGlobal(currentProcedure, nodeName);

	// not found
	if(tmpNode == NULL){
		// Error handling
		printf("\n\nError: st1: Identifier does not exist global!\n");
        printf("Somewhere around line: %ld\n", morph.lineNumber);
		exit(EXIT_FAILURE);
	}
	
	// node is variable?
	if(tmpNode->id != variable){
		// Error handling
		printf("\n\nError: st1: Identifier is not of type variable!\n");
        printf("Somewhere around line: %ld\n", morph.lineNumber);
		exit(EXIT_FAILURE);
	}
	
	// code generation:
		// depends on where it was found - correct?
		// "found in main" could be checked with checking the index of procedure found in
    // get Variable out of node->pObject
    namelistVariable *tmpVariable = tmpNode->pObject;

    if(tmpNode->prodecureIndex == 0)
        code(puAdrVrMain, tmpVariable->offset);
    else if(tmpNode->prodecureIndex == currentProcedure->procedureIndex)
        code(puAdrVrLocl, tmpVariable->offset);
    else
        code(puAdrVrGlob, tmpVariable->offset, tmpNode->prodecureIndex);
		

	return 1;
}

int st2(void){
	debugPrintSMR("st2\n");

	// code generation: store value
	code(storeVal);

	return 1;
}

// if nach condition
int st3(void){
	debugPrintSMR("st3\n");

	// create label				  iJmp is index of jmp command
	pushLabel(labelList, label, (pCode + 1) - codeStartAdress);

	// code generation: jnot mit vorläufiger Relativadresse 0
    code(jnot, 0);

	return 1;
}

// if nach statement
int st4(void){
	debugPrintSMR("st4\n");

	long jumpNotStartOffset = popLabel(labelList);
	
	// calculate relative adress                                    2 xx Byte of jmp not
    long jumpNotWidth = (pCode - codeStartAdress) - jumpNotStartOffset - 2;

    // update the jump not width
    writeToCodeAtPosition(jumpNotWidth, codeStartAdress + jumpNotStartOffset);

	return 1;
}

// while
int st5(void){
	debugPrintSMR("st5\n");

	// calculate relative adress
    pushLabel(labelList, label, pCode - codeStartAdress);

	return 1;
}

// while nach condition
int st6(void){
	debugPrintSMR("st6\n");
    
	pushLabel(labelList, label, (pCode + 1) - codeStartAdress);
	
	// code generation: jnot mit vorläufiger Relativadresse 0
    code(jnot, 0);

	return 1;
}


// while nach statement
int st7(void){
	debugPrintSMR("st7\n");
	
    // get jump not label
    long jumpNotStartOffset = popLabel(labelList);
    
    // calculate relative adress + 3 extra bytes for the jmp-command || +3 jmp Byte || -2 jnot Byte   
    long jumpNotWidth = (pCode - codeStartAdress) - jumpNotStartOffset + 3 - 2;

    // update jump not width
    writeToCodeAtPosition(jumpNotWidth, codeStartAdress + jumpNotStartOffset);



    // ---
	// pop condition label
    long conditionStartOffset = popLabel(labelList);

    // calculate relative adress, so that jmp at end of while gets to first command of condition
                                                    // +1 to get to first cmd  || +2 cause of the 2 jmp bytes
    long conditionWidth = (pCode - codeStartAdress) - conditionStartOffset + 1 + 2;

    // write jump with correct jump width
    code(jmp, -conditionWidth);

	return 1;
}

// while nach statement
int st8(void){
	debugPrintSMR("st8\n");
	int ret = 0;
	char nodeName[strlen(morph.word)];
	strcpy(nodeName, morph.word);

	// search name local
	namelistNode *tmpNode = searchNamelistNodeGlobal(currentProcedure, nodeName);

	// not found
	if(tmpNode == NULL){
		// Error handling
		printf("\n\nError: st8: Identifier does not exist global!\n");
        printf("Somewhere around line: %ld\n", morph.lineNumber);
		exit(EXIT_FAILURE);
	}
	
	// node is variable?
	if(tmpNode->id != procedure){
		// Error handling
		printf("\n\nError: st8: Identifier is not of type procedure!\n");
        printf("Somewhere around line: %ld\n", morph.lineNumber);
		exit(EXIT_FAILURE);

		// return
		return -1;
	}

	namelistProcedure *tmpProcedure = tmpNode->pObject;

	// codegeneration: call procedurenumber
	code(call, tmpProcedure->procedureIndex);

	return 1;
}


int st9(void) {
	debugPrintSMR("st9\n");
	int ret = 0;
	char nodeName[strlen(morph.word)];
	strcpy(nodeName, morph.word);

	// search name local
	namelistNode *tmpNode = searchNamelistNodeGlobal(currentProcedure, nodeName);

	// not found
	if(tmpNode == NULL){
		// Error handling
		printf("\n\nError: st9: Identifier does not exist global!\n");
        printf("Somewhere around line: %ld\n", morph.lineNumber);
		exit(EXIT_FAILURE);
	}
	
	// node is variable?
	if(tmpNode->id != variable){
		// Error handling
		printf("\n\nError: st9: Identifier is not of type variable!\n");
        printf("Somewhere around line: %ld\n", morph.lineNumber);
		exit(EXIT_FAILURE);
	}
	
	// code generation:		
		// depends on where it was found - correct?
		// "found in main" could be checked with checking the index of procedure found in
    // get Variable out of node->pObject
    namelistVariable *tmpVariable = tmpNode->pObject;

    if(tmpNode->prodecureIndex == 0)
        code(puAdrVrMain, tmpVariable->offset);
    else if(tmpNode->prodecureIndex == currentProcedure->procedureIndex)
        code(puAdrVrLocl, tmpVariable->offset);
    else
        code(puAdrVrGlob, tmpVariable->offset, tmpNode->prodecureIndex);
		

		// getVal
		code(getVal);

	return 1;
}

int st10(void){
	debugPrintSMR("st10\n");
	// code generation: putVal
	code(putVal);

	return 1;
}

// expression functions ========================================
int ex1(void){
	debugPrintSMR("ex1\n");
	// code generation: vzMinus
	code(vzMinus);

	return 1;
}

int ex2(void){
	debugPrintSMR("ex2\n");
	// code generation: opAdd
	code(OpAdd);

	return 1;
}

int ex3(void){
	debugPrintSMR("ex3\n");
	// code generation: opSub
	code(OpSub);

	return 1;
}

// term functions ========================================
int te1(void){
	debugPrintSMR("te1\n");
	// code generation: opMul
	code(OpMult);

	return 1;
}

int te2(void){
	debugPrintSMR("te2\n");
	// code generation: opDiv
	code(OpDiv);

	return 1;
}

// factor functions ========================================
// Numberal
int fa1(void){
	debugPrintSMR("fa1\n");
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
		int32_t* tmpArray = (int32_t* )realloc(constArray, constArraySize * sizeof(int32_t));

		if(tmpArray == NULL){
			printf("\n\n\nfailure\n\n\n");
		}
		else{
			constArray = tmpArray;
		}

		// add constant to the array
		constArray[constArraySize-1] = value;
		i = constArraySize - 1;
	}

	// code generation: putConst(ConstIndex)
	code(puConst, i);

	return 1;
}

int fa2(void){
	debugPrintSMR("fa2\n");
	// Ident

	int ret = 0;
	char nodeName[strlen(morph.word)];
	strcpy(nodeName, morph.word);

	// search name local
	namelistNode *tmpNode = searchNamelistNodeGlobal(currentProcedure, nodeName);

	// not found
	if(tmpNode == NULL){
		// Error handling
		printf("\n\nError: fa1: Identifier does not exist global!\n");
        printf("Somewhere around line: %ld\n", morph.lineNumber);
		exit(EXIT_FAILURE);
	}
	
	// node is variable or constant?	
	if(tmpNode->id != variable && tmpNode->id != constant){
		// Error handling
		printf("\n\nError: fa2: Identifier is not of type variable or constant!\n");
        printf("Somewhere around line: %ld\n", morph.lineNumber);
		exit(EXIT_FAILURE);
	}

	// code generation:
	if(tmpNode->id == variable){
		// depends on where it was found - correct?
		// "found in main" could be checked with checking the index of procedure found in

        // get Variable out of node->pObject
        namelistVariable *tmpVariable = tmpNode->pObject;

        if(tmpNode->prodecureIndex == 0)
            code(puAdrVrMain, tmpVariable->offset);
        else if(tmpNode->prodecureIndex == currentProcedure->procedureIndex)
            code(puAdrVrLocl, tmpVariable->offset);
        else
            code(puAdrVrGlob, tmpVariable->offset, tmpNode->prodecureIndex);

	}
	
	if(tmpNode->id == constant){
        // get Variable out of node->pObject
        namelistConst *tmpConst = tmpNode->pObject;
		
        // puConst(index)
		code(puConst, tmpConst->index);
	}		

	return 1;
}


// condition functions ========================================
int co1(void){
	debugPrintSMR("co1\n");

	// code generation: odd
	code(odd);

	return 1;
}

int co2(void){
	debugPrintSMR("co2\n");

	// code generation: save compare operator '='
	// cmpEQ
	compareCase = 2;

	return 1;
}

int co3(void){
	debugPrintSMR("co3\n");

	// save compare operator '#'
	// cmpNE
	compareCase = 3;

	return 1;
}

int co4(void){
	debugPrintSMR("co4\n");

	// save compare operator '<'
	// cmpLT
	compareCase = 4;

	return 1;
}

int co5(void){
	debugPrintSMR("co5\n");

	// save compare operator '<='
	// cmpLE
	compareCase = 5;

	return 1;
}

int co6(void){
	debugPrintSMR("co6\n");

	// save compare operator '>'
	// cmpGT
	compareCase = 6;

	return 1;
}

int co7(void){
	debugPrintSMR("co7\n");

	// save compare operator '>='
	// cmpGE
	compareCase = 7;

	return 1;
}

int co8(void){
	debugPrintSMR("co8\n");

	// code generation: compare operator (before saved)	
	switch(compareCase){
		case 2:
			code(cmpEQ);
			break;

		case 3:		
			code(cmpNE);
			break;

		case 4:		
			code(cmpLT);
			break;

		case 5:		
			code(cmpLE);
			break;

		case 6:
			code(cmpGT);
			break;

		case 7:		
			code(cmpGE);
			break;
		
		default:
			printf("\n\nError: co8: condition: no legal compareCase!\n");
            printf("Somewhere around line: %ld\n", morph.lineNumber);
			exit(EXIT_FAILURE);
	}

	return 1;
}


   
    /*
    // print whole list=================
    list *pList = mainProcedure->pList;
	printf("\n=== main List:\n");
	getFirst(pList);
	do{
		printf("->%s", pList->current->item->pName, pList->current->item->id);
	}while(getNext(pList) != NULL);
    printf("\n\n");
    //==================================
    */