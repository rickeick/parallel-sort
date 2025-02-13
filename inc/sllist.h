#ifndef _SLLIST_H_
#define _SLLIST_H_

typedef struct _sllist_ SLList;

typedef struct _slnode_ SLNode;

SLList *sllCreate();

int sllAppend(SLList *l, void *data);

void *sllGetNext(SLList *l);

int sllDestroy(SLList *l, void (*myfree)(void *));

#endif
