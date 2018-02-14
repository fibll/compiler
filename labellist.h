#ifndef LABELLIST_H 
#define LABELLIST_H

// label item
typedef struct tLABEL
{
    int id;
    long iJmp;
}tLabel;

// label node
typedef struct LABELNODE
{
    struct LABELNODE *next;
    tLabel *item;	
}pLabelNode;

// label list
typedef struct
{
    pLabelNode *first;
    pLabelNode *last;
    pLabelNode *current;
}labellist;

// function prototypes
labellist *createLabellist(void);
int pushHeadLabel(labellist *pList, tLabel *itemIns);
tLabel *popHeadLabel(labellist *pList);
tLabel *getFirstLabel(labellist *pList);
tLabel *getNextLabel(labellist *pList);

// label functions
int pushLabel(labellist *listOfLabels, int id, long iJmp);
long popLabel(labellist *listOfLabels);

#endif