#include "acutest.h"
#include "devices/compactflash/compactflash.h"

void test_Create_dataIsSet(void)
{
    // arrange
    uint8_t data[] = {0, 1, 2, 3, 4};

    // act
    CompactFlash* compactFlash = CF_Create(data);

    // assert 
    TEST_ASSERT(compactFlash->_data == data);
}

TEST_LIST = {
   { "test_Create_dataIsSet", test_Create_dataIsSet },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};