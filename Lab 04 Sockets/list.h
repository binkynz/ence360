#include <stdint.h>

typedef struct node {
	int value;
	struct node* next;
} node_t;

extern node_t* list_new(const int value);
extern void list_add(node_t* const head, const int value);
extern void list_remove(node_t** head, const int value);
extern void list_print(node_t* const head);
extern void list_free(node_t* head);