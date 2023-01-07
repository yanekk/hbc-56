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

void test_dataRequestStatusIsZeroByDefault(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(1);

    // act & assert
    TEST_ASSERT(CF_Read_Status_DataRequest(compactFlash) == false);
}

void test_dataRequestStatusIsZeroAfterSettingSectorCount(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(1);
    CF_Write_SectorCount(compactFlash, 1);

    // act & assert
    TEST_ASSERT(CF_Read_Status_DataRequest(compactFlash) == false);
}

void test_dataRequestStatusIsZeroAfterSettingSectorNumber(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(1);
    CF_Write_SectorNumber(compactFlash, 1);

    // act & assert
    TEST_ASSERT(CF_Read_Status_DataRequest(compactFlash) == false);
}

void test_dataRequestStatusIsZeroAfterSettingSectorNumberAndCount(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(1);
    CF_Write_SectorNumber(compactFlash, 0);
    CF_Write_SectorCount(compactFlash, 1);

    // act & assert
    TEST_ASSERT(CF_Read_Status_DataRequest(compactFlash) == false);
}

void test_errorInvalidSectorIsReadCommandIsSentWithoutSettingSectorNumber(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(1);
    CF_Write_SectorCount(compactFlash, 1);
    CF_Write_Command_ReadSectors(compactFlash);

    // act & assert
    TEST_ASSERT(CF_Read_Error_InvalidSector(compactFlash) == true);
}

void test_errorInvalidSectorIsReadCommandIsSentWithoutSettingSectorCount(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(1);
    CF_Write_SectorNumber(compactFlash, 1);
    CF_Write_Command_ReadSectors(compactFlash);

    // act & assert
    TEST_ASSERT(CF_Read_Error_InvalidSector(compactFlash) == true);
}

void test_noErrorInvalidSectorIsReadCommandIsSentWithSectorCountAndNumberSet(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(1);
    CF_Write_SectorNumber(compactFlash, 1);
    CF_Write_SectorCount(compactFlash, 1);
    CF_Write_Command_ReadSectors(compactFlash);

    // act & assert
    TEST_ASSERT(CF_Read_Error_InvalidSector(compactFlash) == false);
}

void test_dataIsReadyToBeRead(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(1);
    CF_Write_SectorNumber(compactFlash, 0);
    CF_Write_SectorCount(compactFlash, 1);
    CF_Write_Command_ReadSectors(compactFlash);

    // act & assert
    TEST_ASSERT(CF_Read_Status_DataRequest(compactFlash) == true);
}

void test_firstByteIsReceivedForFirstSector(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(1);
    CF_Write_SectorNumber(compactFlash, 0);
    CF_Write_SectorCount(compactFlash, 1);
    CF_Write_Command_ReadSectors(compactFlash);

    // act & assert
    TEST_ASSERT(CF_Read_Data(compactFlash) == compactFlash->_data[0]);
}

void test_secondByteIsReceivedForFirstSector(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(1);
    CF_Write_SectorNumber(compactFlash, 0);
    CF_Write_SectorCount(compactFlash, 1);
    CF_Write_Command_ReadSectors(compactFlash);
    CF_Read_Data(compactFlash); // read first byte

    // act & assert
    TEST_ASSERT(CF_Read_Data(compactFlash) == compactFlash->_data[1]);
}

void test_firstByteIsReceivedForSecondSector(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(1);
    CF_Write_SectorNumber(compactFlash, 1);
    CF_Write_SectorCount(compactFlash, 1);
    CF_Write_Command_ReadSectors(compactFlash);

    // act & assert
    TEST_ASSERT(CF_Read_Data(compactFlash) == compactFlash->_data[SECTOR_SIZE]);
}

void test_receivingAllBytesInFirstSectorUnsetsDataRequestStatus(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(1);
    CF_Write_SectorNumber(compactFlash, 0);
    CF_Write_SectorCount(compactFlash, 1);
    CF_Write_Command_ReadSectors(compactFlash);

    // act & assert
    for(uint16_t i = 0; i < SECTOR_SIZE-1; i++) {
        CF_Read_Data(compactFlash);
        TEST_ASSERT(CF_Read_Status_DataRequest(compactFlash) == true);
    }
    CF_Read_Data(compactFlash);
    TEST_ASSERT(CF_Read_Status_DataRequest(compactFlash) == false);
}

void test_receivingAllBytesInSecondSectorUnsetsDataRequestStatus(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(1);
    CF_Write_SectorNumber(compactFlash, 1);
    CF_Write_SectorCount(compactFlash, 1);
    CF_Write_Command_ReadSectors(compactFlash);

    // act & assert
    for(uint16_t i = 0; i < SECTOR_SIZE-1; i++) {
        CF_Read_Data(compactFlash);
        TEST_ASSERT(CF_Read_Status_DataRequest(compactFlash) == true);
    }
    CF_Read_Data(compactFlash);
    TEST_ASSERT(CF_Read_Status_DataRequest(compactFlash) == false);
}

void test_receivingAllBytesInMultipleSectorsUnsetsDataRequestStatus(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(1);
    CF_Write_SectorNumber(compactFlash, 1);
    CF_Write_SectorCount(compactFlash, 2);
    CF_Write_Command_ReadSectors(compactFlash);

    // act & assert
    for(uint16_t i = 0; i < SECTOR_SIZE * 2 - 1; i++) {
        CF_Read_Data(compactFlash);
        TEST_ASSERT(CF_Read_Status_DataRequest(compactFlash) == true);
    }
    CF_Read_Data(compactFlash);
    TEST_ASSERT(CF_Read_Status_DataRequest(compactFlash) == false);
}

void test_notEmulatedStatusBitsHaveAlwaysDefaultValues(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(1);

    // act & assert
    TEST_ASSERT(CF_Read_Status_Busy(compactFlash) == false);
    TEST_ASSERT(CF_Read_Status_Ready(compactFlash) == true);
    TEST_ASSERT(CF_Read_Status_DriveWriteFault(compactFlash) == false);
    TEST_ASSERT(CF_Read_Status_MemoryCardReady(compactFlash) == true);
    TEST_ASSERT(CF_Read_Status_CorrectableDataError(compactFlash) == false);
}

void test_notEmulatedErrorBitsHaveAlwaysDefaultValues(void)
{
    // arrange
    CompactFlash* compactFlash = initTest(1);

    // act & assert
    TEST_ASSERT(CF_Read_Status_Error(compactFlash) == false);
    TEST_ASSERT(CF_Read_Error_BadBlock(compactFlash) == false);
    TEST_ASSERT(CF_Read_Error_UncorrectableError(compactFlash) == false);
    TEST_ASSERT(CF_Read_Error_InvalidCommand(compactFlash) == false);
    TEST_ASSERT(CF_Read_Error_GeneralError(compactFlash) == false);
}
/*
TODO:
  LDA #$04      ; Reset
  STA CFCMD

  LDA #$01      ; LD features register to enable 8 bit
  STA CFFEAT
  
  LDA #$EF      ; Send set features command
  STA CFCMD
*/
TEST_LIST = {
   { "test_Create_dataIsSet", test_Create_dataIsSet },
   { "test_Destroy_compactFlashIsFreed", test_Destroy_compactFlashIsFreed },
   { "test_sectorNumberIsStored", test_sectorNumberIsStored },
   { "test_dataRequestStatusIsZeroByDefault", test_dataRequestStatusIsZeroByDefault },
   { "test_dataRequestStatusIsZeroAfterSettingSectorCount", test_dataRequestStatusIsZeroAfterSettingSectorCount },
   { "test_dataRequestStatusIsZeroAfterSettingSectorNumber", test_dataRequestStatusIsZeroAfterSettingSectorNumber },
   { "test_dataRequestStatusIsZeroAfterSettingSectorNumberAndCount", test_dataRequestStatusIsZeroAfterSettingSectorNumberAndCount },
   { "test_errorInvalidSectorIsReadCommandIsSentWithoutSettingSectorNumber", test_errorInvalidSectorIsReadCommandIsSentWithoutSettingSectorNumber },
   { "test_errorInvalidSectorIsReadCommandIsSentWithoutSettingSectorCount", test_errorInvalidSectorIsReadCommandIsSentWithoutSettingSectorCount },   
   { "test_noErrorInvalidSectorIsReadCommandIsSentWithSectorCountAndNumberSet", test_noErrorInvalidSectorIsReadCommandIsSentWithSectorCountAndNumberSet },   
   { "test_dataIsReadyToBeRead", test_dataIsReadyToBeRead },   
   { "test_firstByteIsReceivedForFirstSector", test_firstByteIsReceivedForFirstSector },  
   { "test_secondByteIsReceivedForFirstSector", test_secondByteIsReceivedForFirstSector },  
   { "test_firstByteIsReceivedForSecondSector", test_firstByteIsReceivedForSecondSector },  
   { "test_receivingAllBytesInFirstSectorUnsetsDataRequestStatus", test_receivingAllBytesInFirstSectorUnsetsDataRequestStatus },  
   { "test_receivingAllBytesInSecondSectorUnsetsDataRequestStatus", test_receivingAllBytesInSecondSectorUnsetsDataRequestStatus },  
   { "test_receivingAllBytesInMultipleSectorsUnsetsDataRequestStatus", test_receivingAllBytesInMultipleSectorsUnsetsDataRequestStatus },  
   { "test_notEmulatedStatusBitsHaveAlwaysDefaultValues", test_notEmulatedStatusBitsHaveAlwaysDefaultValues },  
   { "test_notEmulatedErrorBitsHaveAlwaysDefaultValues", test_notEmulatedErrorBitsHaveAlwaysDefaultValues },  
   { NULL, NULL }     /* zeroed record marking the end of the list */
};