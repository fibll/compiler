#include <stdio.h>
#include <stdlib.h>
#include "namelist.h"

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

int insertTail(list *pList, void *itemIns)
{
	node *pTmp = malloc(sizeof(node));
	pTmp->next = NULL;
	if(pTmp)
	{
		pTmp->item = itemIns;
		if(pList->first == NULL)
		{
			// Liste ist noch Leer
			pList->first = pList->last = pTmp;
			//pList->first = pTmp;
			//pList->last = pTmp;
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

void *getFirst (list *pList)
{
	node *pTmp;
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

void *getNext(list *pList)
{
	node *pTmp;
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

namelistNode *createNamelistNode(char *nodeName)
{
	;
}

namelistConst *createNamelistConst(long value)
{
	;
}

namelistConst *searchConst(long value)
{
	;
}

int createNamelistVariable(void)
{
	;
}

namelistProcedure *createNamelistProcedure(namelistProcedure pParentProcedure)
{
	;
}


namelistNode *searchNamelistNode(namelistProcedure pProcedure, char *nodeName)
{
	;
}

namelistNode *searchNamelistNodeGlobal(char *nodeName)
{
	;
}
