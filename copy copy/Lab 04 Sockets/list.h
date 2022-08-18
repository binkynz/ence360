#include <stdint.h>

typedef struct list {
    int value;
    struct list* next;
} list_t;

extern list_t* list_create(int value);
extern void list_append(list_t** head, int value);
extern void list_remove(list_t** head, int value);
extern void list_free(list_t** head);
extern void list_print(list_t* head);