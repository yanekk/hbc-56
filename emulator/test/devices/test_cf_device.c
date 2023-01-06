#include "acutest.h"
#include "devices/cf_device.h"
#include "devices/device.h"
#include "devices/compactflash/compactflash.h"

#define BASE_ADDRESS 0x1000

typedef struct CompactFlashSpy {
    bool isCreated;
    bool isDestroyed;

    bool statusError;

    uint8_t sectorCount;
    CompactFlash data;
} CompactFlashSpy;

CompactFlashSpy compactFlashSpy = {};

CompactFlash cfCard = {};

CompactFlash* CompactFlash_Create(const uint8_t *data) {
    compactFlashSpy.isCreated = true;
    return &compactFlashSpy.data;
}

bool CompactFlash_Read_Status_Error(CompactFlash *device) {
    return compactFlashSpy.statusError;
}

void CompactFlash_Destroy(CompactFlash * device) {
      compactFlashSpy.isDestroyed = true;
}

void CompactFlash_SectorCount_Write(CompactFlash *device, uint8_t sectorCount) {
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

void test_reset() {
    compactFlashSpy = (CompactFlashSpy){0};
}

void test_createDevice_nameIsSet(void)
{
    // arrange
    test_reset();

    // act
    HBC56Device testDevice = createCompactFlashDevice(0, NULL);

    // assert 
    TEST_ASSERT(strcmp(testDevice.name, "CompactFlash") == 0);
}

void test_createDevice_cfCardIsCreated(void)
{
    // arrange
    test_reset();

    // act
    createCompactFlashDevice(0, NULL);

    // assert 
    TEST_ASSERT(compactFlashSpy.isCreated == true);
}

void test_destroyDevice_cfCardIsDestroyed(void)
{
    // arrange
    test_reset();

    // act
    HBC56Device testDevice = createCompactFlashDevice(0, NULL);
    destroyDevice(&testDevice);

    // assert 
    TEST_ASSERT(compactFlashSpy.isDestroyed == true);
}

void test_readDevice_CF_STAT_zeroByDefault(void)
{
    // arrange
    test_reset();

    HBC56Device testDevice = createCompactFlashDevice(BASE_ADDRESS, NULL);
    
    uint8_t expectedStatus = 0b00000000;
    uint8_t actualStatus = 0;

    // act
    readDevice(&testDevice, BASE_ADDRESS + CF_STAT, &actualStatus, 0);

    // assert
    TEST_ASSERT(actualStatus == expectedStatus);
}

void test_readDevice_CF_STAT_statusErrorSetBit0(void)
{
    // arrange
    test_reset();

    HBC56Device testDevice = createCompactFlashDevice(BASE_ADDRESS, NULL);
    
    
    compactFlashSpy.statusError = true;
    uint8_t expectedStatus = 0b00000001;

    uint8_t actualStatus = 0;

    // act
    readDevice(&testDevice, BASE_ADDRESS + CF_STAT, &actualStatus, 0);

    // assert
    TEST_ASSERT(actualStatus == expectedStatus);
}

TEST_LIST = {
   { "test_createDevice_nameIsSet", test_createDevice_nameIsSet },
   { "test_createDevice_cfCardIsCreated", test_createDevice_cfCardIsCreated },
   { "test_destroyDevice_cfCardIsDestroyed", test_destroyDevice_cfCardIsDestroyed },
   { "test_readDevice_CF_STAT_zeroByDefault", test_readDevice_CF_STAT_zeroByDefault },
   { "test_readDevice_CF_STAT_statusErrorSetBit0", test_readDevice_CF_STAT_statusErrorSetBit0 },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};