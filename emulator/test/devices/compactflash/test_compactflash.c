#include "acutest.h"
#include "devices/compactflash/compactflash.h"
#include <stdlib.h>

CompactFlash* initTest(uint8_t sectorCount) {
    uint16_t dataSize = 512 * sectorCount;
    uint8_t* data = malloc(sizeof(uint8_t) * dataSize);

    for(uint16_t i = 0; i < dataSize; i++) {
        data[i] = rand();
    }
    return CF_Create(data);
}

void test_Create_dataIsSet(void)
{
    // arrange
    uint8_t data[] = {0, 1, 2, 3, 4};

    // act
    CompactFlash* compactFlash = CF_Create(data);

    // assert 
    TEST_ASSERT(compactFlash->_data == data);
}

void test_Destroy_compactFlashIsFreed(void)
{
    // arrange
    uint8_t data[] = {0, 1, 2, 3, 4};
    CompactFlash* compactFlash = CF_Create(data);

    // act
    CF_Destroy(compactFlash);

    // assert 
    TEST_ASSERT(compactFlash->_data != data);
}

void test_sectorNumberIsStored(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(0);

    // act
    CF_Write_SectorNumber(compactFlash, 0x123456);

    // assert 
    TEST_ASSERT(CF_Read_SectorNumber(compactFlash) == 0x123456);
}

// void test_Read_correctNumberOfBytesIsReturned(void)
// {
//     // arrange
//     CompactFlash* compactFlash = initTest(1);

//     // act
//     CF_Write_SectorCount(compactFlash, 1);
//     CF_Write_SectorNumber(compactFlash, 0);

//     CF_Write_Command_ReadSectors(compactFlash);

//     uint16_t byteCount = 0;
//     while(CF_Read_Status_DataRequest(compactFlash)) {
//         CF_Read_Data(compactFlash);
//         byteCount++;
//     }

//     // assert 
//     TEST_ASSERT(byteCount == 512);
// }

TEST_LIST = {
   { "test_Create_dataIsSet", test_Create_dataIsSet },
   { "test_Destroy_compactFlashIsFreed", test_Destroy_compactFlashIsFreed },
   { "test_sectorNumberIsStored", test_sectorNumberIsStored },
//    { "test_Read_correctNumberOfBytesIsReturned", test_Read_correctNumberOfBytesIsReturned },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};