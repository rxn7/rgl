#ifndef __RGL_LIST_H
#define __RGL_LIST_H

#include "rgl_common.h"

typedef struct rglLinkedListNode {
	void *data;
	struct rglLinkedListNode *next;
} rglLinkedListNode;

rglLinkedListNode *rglLinkedListNodeCreate(void *data);

typedef struct rglLinkedList {
	rglLinkedListNode *head;
	u32 length;
} rglLinkedList;

void rglLinkedListCreate(rglLinkedList *list);
void rglLinkedListClear(rglLinkedList *list, b8 free_data);
void rglLinkedListAdd(rglLinkedList *list, void *data);
void rglLinkedListDelete(rglLinkedList *list, void *data, b8 free_data);

#endif /* __RGL_LIST_H */
