#ifndef _SLLIST_C_
#define _SLLIST_C_

#include <stdlib.h>

#include "sllist.h"

typedef struct _sllist_ {
    SLNode *first;
    SLNode *cur;
} SLList;

typedef struct _slnode_ {
    void *data;
    SLNode *next;
} SLNode;

SLList *sllCreate() {
    SLList *l;
    l = (SLList *)malloc(sizeof(SLList));
    if (l != NULL) {
        l->first = NULL;
        l->cur = NULL;
        return l;
    }
    return NULL;
}

int sllAppend(SLList *l, void *data) {
    SLNode *last, *newnode;
    if (l != NULL) {
        newnode = (SLNode *)malloc(sizeof(SLNode));
        if (newnode != NULL) {
            if (l->first != NULL) {
                last = l->first;
                while (last->next != NULL) {
                    last = last->next;
                }
                last->next = newnode;
            }
            else {
                l->first = newnode;
            }
            newnode->data = data;
            newnode->next = NULL;
            return 1;
        }
    }
    return 0;
}

void *sllGetNext(SLList *l) {
    if (l != NULL) {
        if (l->cur != NULL) {
            l->cur = l->cur->next;
            if (l->cur != NULL) {
                return l->cur->data;
            }
        }
        if (l->first != NULL) {
            l->cur = l->first;
            return l->cur->data;
        }
    }
    return NULL;
}

int sllDestroy(SLList *l, void (*myfree)(void *)) {
    SLNode *cur, *prev;
    if (l != NULL) {
        if (l->first == NULL) {
            cur = l->first;
            while (cur != NULL) {
                prev = cur;
                cur = cur->next;
                myfree(prev->data);
                free(prev);
            }
            free(l);
            return 1;
        }
    }
    return 0;
}

#endif
