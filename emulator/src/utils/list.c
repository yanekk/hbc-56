#include <stdint.h>
#include <stdlib.h>

#include "utils/list.h"

struct List {
    uint16_t size;
    void** items;
};

List* List_Create() {
    List* list = malloc(sizeof(List));
    list->size = 0;
    list->items = calloc(sizeof(void*), 0);

    return list == NULL ? NULL : list;
}

uint16_t List_Size(List* list) {
    return list->size;
}

void List_Append(List* list, void* item) {
    list->size++;
    list->items = realloc(list->items, list->size * sizeof(void*));
    list->items[list->size-1] = item;
}

void List_Destroy(List* list) {
    list->size = 0;
    if(list->items != NULL)
        free(list->items);
    free(list);
}

void* List_At(List* list, uint16_t index) {
    return list->items[index];
}