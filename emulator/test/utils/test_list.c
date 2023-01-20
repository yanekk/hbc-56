#include <stdint.h>
#include <stdbool.h>

#include "acutest.h"
#include "utils/int.h"

typedef struct {
    uint16_t size;
    void** items;
} List;

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

void test_size_returnsSize() {
    // arrange
    List* list = List_Create();

    // act & assert
    TEST_CHECK(List_Size(list) == 0);

    // cleanup
    List_Destroy(list);
}

void test_append_sizeIsIncreased() {
    // arrange
    List* list = List_Create();
    uint8_t item = 10;

    // act
    List_Append(list, &item);
    
    // assert
    TEST_CHECK(List_Size(list) == 1);

    // cleanup
    List_Destroy(list);
}

void test_at_elementIsReturned() {
    // arrange
    List* list = List_Create();
    uint8_t item = 0xFB;
    List_Append(list, &item);

    // act
    uint8_t actualItem = *(uint8_t*)List_At(list, 0);

    // assert
    TEST_CHECK(actualItem == item);

    // cleanup
    List_Destroy(list);
}

void test_listAllowsToAddAndGetItems() {
    // arrange
    List* list = List_Create();

    // act
    for(uint8_t i = 0; i < 10; i++) {
        uint8_t* j = malloc(sizeof(uint8_t));
        *j = i;
        List_Append(list, j);
    }

    uint8_t sum = 0;
    for(uint16_t i = 0; i < List_Size(list); i++) {
        uint8_t* item = List_At(list, i);
        sum += *item;
        free(item);
    }

    // assert
    TEST_CHECK(sum == 45);

    // cleanup
    List_Destroy(list);
}

TEST_LIST = {
    { "test_size_returnsSize", test_size_returnsSize },
    { "test_append_sizeIsIncreased", test_append_sizeIsIncreased },
    { "test_at_elementIsReturned", test_at_elementIsReturned },
    { "test_listAllowsToAddAndGetItems", test_listAllowsToAddAndGetItems },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};
