#include <stdint.h>
#include <stdbool.h>

#include "acutest.h"
#include "utils/int.h"

typedef struct {
    uint16_t size;
} List;

List* List_Create() {
    List* list = malloc(sizeof(List));
    list->size = 0;
    return list == NULL ? NULL : list;
}

uint16_t List_Size(List* list) {
    return list->size;
}

void List_Destroy(List* list) {
    list->size = 0;
    free(list);
}

void test_size_returnsSize() {
    // arrange
    List* list = List_Create();

    // act & assert
    TEST_CHECK(List_Size(list) == 0);

    // cleanup
    List_Destroy(list);
}

TEST_LIST = {
    { "test_size_returnsSize", test_size_returnsSize },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};
