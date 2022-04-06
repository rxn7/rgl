#include "rgl_list.h"

rglLinkedListNode 
*rglLinkedListNodeCreate(void *data) {
	RGL_ASSERT_VALID_PTR(data);
	rglLinkedListNode *node = malloc(sizeof(rglLinkedListNode));

	node->data = data;
	node->next = NULL;

	return node;
}

void
rglLinkedListNodeDestroy(rglLinkedListNode *node, b8 free_data) {
	RGL_ASSERT_VALID_PTR(node);

	if(free_data) {
		RGL_ASSERT_VALID_PTR(node->data);
		free(node->data);
	}

	free(node);
}

void
rglLinkedListCreate(rglLinkedList *list) {
	RGL_ASSERT_VALID_PTR(list);

	list->head = NULL;
	list->length = 0;
}

void
rglLinkedListClear(rglLinkedList *list, b8 free_data) {
	RGL_ASSERT_VALID_PTR(list);

	rglLinkedListNode *current = list->head, *next = current;
	while(current != NULL) {
		next = current->next;

		rglLinkedListNodeDestroy(current, free_data);
		
		current = next;
	}

	list->head = NULL;
	list->length = 0;
}

void 
rglLinkedListAdd(rglLinkedList *list, void *data) {
	RGL_ASSERT_VALID_PTR(list);
	RGL_ASSERT_VALID_PTR(data);

	if(list->head == NULL) {
		list->head = rglLinkedListNodeCreate(data);
	} else {
		rglLinkedListNode *current = list->head;
		while(current->next != NULL) {
			current = current->next;
		}
		current->next = rglLinkedListNodeCreate(data);
	}

	list->length++;
}

void
rglLinkedListDelete(rglLinkedList *list, void *data, b8 free_data) {
	RGL_ASSERT_VALID_PTR(list);
	RGL_ASSERT_VALID_PTR(data);

	rglLinkedListNode *current = list->head, *previous = current;

	while(current != NULL) {
		if(current->data == data) {
			previous->next = current->next;

			if(current == list->head) {
				list->head = current->next;
			}

			rglLinkedListNodeDestroy(current, free_data);
			list->length--;

			return;
		}

		previous = current;
		current = current->next;
	}
}
