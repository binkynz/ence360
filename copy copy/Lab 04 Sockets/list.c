#include <stdlib.h>
#include <stdio.h>

#include "list.h"

list_t* list_create(int value) {
    list_t* new = malloc(sizeof(list_t));
    new->value = value;
    new->next = NULL;
    return new;
}

void list_append(list_t** head, int value) {
    list_t* new = list_create(value);
    new->next = *head;
    *head = new;
}

void list_remove(list_t** head, int value) {
    if ((*head)->value == value) {
        list_t* temp = (*head)->next;
        free(*head);
        *head = temp;
    } else {
        list_t* prev = NULL;
        for (list_t* node = *head; node != NULL; node = node->next) {
            if (node->value != value) {
                prev = node;
                continue;
            }

            prev->next = node->next;
            free(node);
            break;     
        }
    }
}

void list_free(list_t** head) {
    for (list_t* node = *head; node != NULL;) {
        list_t* temp = node->next;
        free(node);
        node = temp;
    }

    *head = NULL;
}

void list_print(list_t* head) {
    for (list_t* node = head; node != NULL; node = node->next)
        printf("%d, ", node->value);
    printf("\n");
}