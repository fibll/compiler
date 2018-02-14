#include <stdlib.h>

#include "labellist.h"

// Basic List Functions ---
labellist *createLabellist(void)
{
	labellist *pTmp;
	pTmp = malloc(sizeof(labellist));
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
	// return (int)pTmp;
    return 1;
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