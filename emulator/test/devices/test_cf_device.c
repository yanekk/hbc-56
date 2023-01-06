#include "acutest.h"
#include "devices/cf_device.h"
#include "devices/device.h"
#include "devices/compactflash/compactflash.h"

#define BASE_ADDRESS 0x1000

typedef struct CompactFlashSpy {
    bool isCreated;
    bool isDestroyed;

    bool statusError;
    bool statusCorrectableDataError;
    bool statusDataRequest;
    bool statusMemoryCardReady;
    bool statusDriveWriteFault;
    bool statusReady;
    bool statusBusy;

    bool errorBadBlock;
    bool errorUncorrectableError;
    bool errorInvalidSector;
    bool errorInvalidCommand;
    bool errorGeneralError;

    uint8_t sectorCount;
    uint8_t * data;
} CompactFlashSpy;

CompactFlashSpy compactFlashSpy = {};
CompactFlash testCfCard = {};

HBC56Device testDevice;

CompactFlash* CompactFlash_Create(const uint8_t *data) {
    compactFlashSpy.isCreated = true;
    compactFlashSpy.data = (uint8_t*)data;
    return &testCfCard;
}

bool CompactFlash_Read_Status_Busy(CompactFlash *device) {
    return compactFlashSpy.statusBusy;
}

bool CompactFlash_Read_Status_Ready(CompactFlash *device) {
    return compactFlashSpy.statusReady;
}

bool CompactFlash_Read_Status_DriveWriteFault(CompactFlash *device) {
    return compactFlashSpy.statusDriveWriteFault;
}

bool CompactFlash_Read_Status_MemoryCardReady(CompactFlash *device) {
    return compactFlashSpy.statusMemoryCardReady;
}

bool CompactFlash_Read_Status_DataRequest(CompactFlash *device) {
    return compactFlashSpy.statusDataRequest;
}

bool CompactFlash_Read_Status_CorrectableDataError(CompactFlash *device) {
    return compactFlashSpy.statusCorrectableDataError;
}

bool CompactFlash_Read_Status_Error(CompactFlash *device) {
    return compactFlashSpy.statusError;
}

bool CompactFlash_Read_Error_BadBlock(CompactFlash *device) {
    return compactFlashSpy.errorBadBlock;
}

bool CompactFlash_Read_Error_UncorrectableError(CompactFlash *device) {
    return compactFlashSpy.errorUncorrectableError;
}

bool CompactFlash_Read_Error_InvalidSector(CompactFlash *device) {
    return compactFlashSpy.errorInvalidSector;
}

bool CompactFlash_Read_Error_InvalidCommand(CompactFlash *device) {
    return compactFlashSpy.errorInvalidCommand;
}

bool CompactFlash_Read_Error_GeneralError(CompactFlash *device) {
    return compactFlashSpy.errorGeneralError;
}

void CompactFlash_Destroy(CompactFlash * device) {
      compactFlashSpy.isDestroyed = true;
}

void CompactFlash_Write_SectorCount(CompactFlash *device, uint8_t sectorCount) {
    compactFlashSpy.sectorCount = sectorCount;
}

// uint8_t CompactFlash_SectorNumber_Write(CompactFlash *device, uint8_t number, uint8_t offset) {
//     // CFLBA0  = CFBASE + $03		; LBA bits 0-7 (R/W, LBA mode) => offset 0
//     // CFLBA1  = CFBASE + $04		; LBA bits 8-15 (R/W, LBA mode) => offset 7
//     // CFLBA2  = CFBASE + $05		; LBA bits 16-23 (R/W, LBA mode) => offset 15
//     // CFLBA3  = CFBASE + $06		; LBA bits 24-27 (R/W, LBA mode) => offset 23 (4 lower bits only!)
//     return 0;
// }

// uint8_t CompactFlash_Command_ReadSector(CompactFlash *device) {
//     // LDA #$20                ; Read command
//     // STA CFCMD			       		;Send read command
//     return 0;
// }

// uint8_t CompactFlash_Read_Byte(CompactFlash *device) {
//     // LDA CFDATA
//     return 0;
// }


void testInit() {
    compactFlashSpy = (CompactFlashSpy){0};
    testDevice = createCompactFlashDevice(BASE_ADDRESS, NULL);
}

uint8_t testRead(uint8_t offset) {
    uint8_t value = 0;
    readDevice(&testDevice, BASE_ADDRESS + offset, &value, 0);
    return value;
}

void test_createDevice_nameIsSet(void)
{
    // arrange
    compactFlashSpy = (CompactFlashSpy){0};

    // act
    HBC56Device testDevice = createCompactFlashDevice(0, NULL);

    // assert 
    TEST_ASSERT(strcmp(testDevice.name, "CompactFlash") == 0);
}

void test_createDevice_cfCardIsCreated(void)
{
    // arrange
    compactFlashSpy = (CompactFlashSpy){0};

    // act
    createCompactFlashDevice(0, NULL);

    // assert 
    TEST_ASSERT(compactFlashSpy.isCreated == true);
}

void test_createDevice_dataIsPassedToCfCard(void)
{
    // arrange
    compactFlashSpy = (CompactFlashSpy){0};
    uint8_t data[] = {0, 1, 2, 3, 4};

    // act
    createCompactFlashDevice(0, data);

    // assert 
    TEST_ASSERT(compactFlashSpy.data == data);
}

void test_destroyDevice_cfCardIsDestroyed(void)
{
    // arrange
    testInit(); 

    // act
    destroyDevice(&testDevice);

    // assert 
    TEST_ASSERT(compactFlashSpy.isDestroyed == true);
}

void test_readDevice_CF_STAT_zeroByDefault(void)
{
    // arrange
    testInit(); 
    
    // act
    uint8_t actualStatus = testRead(CF_STAT);

    // assert
    TEST_ASSERT(actualStatus ==  0b00000000);
}

void test_readDevice_CF_STAT_statusErrorSetBit0(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.statusError = true;

    // act & assert
    TEST_ASSERT(testRead(CF_STAT) == 0b00000001);
}

void test_readDevice_CF_STAT_statusCorrectableDataErrorSetBit2(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.statusCorrectableDataError = true;

    // act & assert
    TEST_ASSERT(testRead(CF_STAT) == 0b00000100);
}

void test_readDevice_CF_STAT_statusDataRequestSetBit3(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.statusDataRequest = true;

    // act & assert
    TEST_ASSERT(testRead(CF_STAT) == 0b00001000);
}

void test_readDevice_CF_STAT_statusMemoryCardReadySetBit4(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.statusMemoryCardReady = true;

    // act & assert
    TEST_ASSERT(testRead(CF_STAT) == 0b00010000);
}

void test_readDevice_CF_STAT_statusDriveWriteFaultSetBit5(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.statusDriveWriteFault = true;

    // act & assert
    TEST_ASSERT(testRead(CF_STAT) == 0b00100000);
}

void test_readDevice_CF_STAT_statusReadySetBit6(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.statusReady = true;

    // act & assert
    TEST_ASSERT(testRead(CF_STAT) == 0b01000000);
}

void test_readDevice_CF_STAT_statusBusySetBit7(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.statusBusy = true;

    // act & assert
    TEST_ASSERT(testRead(CF_STAT) == 0b10000000);
}

void test_readDevice_CF_ERR_errorBadBlockSetBit7(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.errorBadBlock = true;

    // act & assert
    TEST_ASSERT(testRead(CF_ERR) == 0b10000000);
}

void test_readDevice_CF_ERR_errorUncorrectableErrorSetBit6(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.errorUncorrectableError = true;

    // act & assert
    TEST_ASSERT(testRead(CF_ERR) == 0b01000000);
}

void test_readDevice_CF_ERR_errorInvalidSectorSetBit4(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.errorInvalidSector = true;

    // act & assert
    TEST_ASSERT(testRead(CF_ERR) == 0b00010000);
}

void test_readDevice_CF_ERR_errorInvalidCommandSetBit2(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.errorInvalidCommand = true;

    // act & assert
    TEST_ASSERT(testRead(CF_ERR) == 0b00000100);
}

void test_readDevice_CF_ERR_errorGeneralErrorSetBit0(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.errorGeneralError = true;

    // act & assert
    TEST_ASSERT(testRead(CF_ERR) == 0b00000001);
}

TEST_LIST = {
   { "test_createDevice_nameIsSet", test_createDevice_nameIsSet },
   { "test_createDevice_cfCardIsCreated", test_createDevice_cfCardIsCreated },
   { "test_createDevice_dataIsPassedToCfCard", test_createDevice_dataIsPassedToCfCard },
   { "test_destroyDevice_cfCardIsDestroyed", test_destroyDevice_cfCardIsDestroyed },
   { "test_readDevice_CF_STAT_zeroByDefault", test_readDevice_CF_STAT_zeroByDefault },
   { "test_readDevice_CF_STAT_statusErrorSetBit0", test_readDevice_CF_STAT_statusErrorSetBit0 },
   { "test_readDevice_CF_STAT_statusCorrectableDataErrorSetBit2", test_readDevice_CF_STAT_statusCorrectableDataErrorSetBit2 },
   { "test_readDevice_CF_STAT_statusDataRequestSetBit3", test_readDevice_CF_STAT_statusDataRequestSetBit3 },
   { "test_readDevice_CF_STAT_statusMemoryCardReadySetBit4", test_readDevice_CF_STAT_statusMemoryCardReadySetBit4 },
   { "test_readDevice_CF_STAT_statusReadySetBit6", test_readDevice_CF_STAT_statusReadySetBit6 },
   { "test_readDevice_CF_STAT_statusBusySetBit7", test_readDevice_CF_STAT_statusBusySetBit7 },
   { "test_readDevice_CF_ERR_errorBadBlockSetBit7", test_readDevice_CF_ERR_errorBadBlockSetBit7 },
   { "test_readDevice_CF_ERR_errorUncorrectableErrorSetBit6", test_readDevice_CF_ERR_errorUncorrectableErrorSetBit6 },
   { "test_readDevice_CF_ERR_errorInvalidSectorSetBit4", test_readDevice_CF_ERR_errorInvalidSectorSetBit4 },
   { "test_readDevice_CF_ERR_errorInvalidCommandSetBit2", test_readDevice_CF_ERR_errorInvalidCommandSetBit2 },
   { "test_readDevice_CF_ERR_errorGeneralErrorSetBit0", test_readDevice_CF_ERR_errorGeneralErrorSetBit0 },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};