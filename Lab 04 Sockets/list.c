#include <stdlib.h>
#include <stdio.h>

#include "list.h"

node_t* list_new(const int value) {
	node_t* new = malloc(sizeof(node_t));
	new->value = value;
	new->next = NULL;
	printf("list add: loc: %p, val: %d\n", new, new->value);
	return new;
}

void list_add(node_t* const head, const int value) {
	node_t* item;
	for (item = head; item->next != NULL; item = item->next);
	item->next = list_new(value);
}

void list_remove(node_t** head, const int value) {
	node_t* pitem = NULL, *item;
	for (item = *head; item != NULL; item = item->next) {
		if (item->value == value) {
			printf("list remove: loc: %p, val: %d\n", item, item->value);

			if (pitem == NULL)
				*head = (*head)->next;
			else
				pitem->next = item->next;

			free(item);
			break;
		}

		pitem = item;
	}
}

void list_print(node_t* const head) {
	node_t* item;
	for (item = head; item != NULL; item = item->next) {
		printf("list: loc: %p, val: %d\n", item, item->value);
	}
}

void list_free(node_t* head) {
	node_t* item;
	for (item = head; item != NULL;) {
		node_t* temp = item->next;
		free(item);
		item = temp;
	}
}