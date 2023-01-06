#include "acutest.h"
#include "devices/cf_device.h"
#include "devices/device.h"
#include "devices/compactflash/compactflash.h"

#define BASE_ADDRESS 0x1000

typedef struct CompactFlashSpy {
    uint8_t status;
    bool isCreated;
    bool isDestroyed;
    CompactFlash data;
} CompactFlashSpy;

CompactFlashSpy compactFlashSpy = {};

CompactFlash cfCard = {};

CompactFlash* CompactFlash_Create(uint8_t * data) {
    compactFlashSpy.isCreated = true;
    return &compactFlashSpy.data;
}

uint8_t CompactFlash_Status(CompactFlash *device) {
    return compactFlashSpy.status;
}

void CompactFlash_Destroy(CompactFlash * device) {
      compactFlashSpy.isDestroyed = true;
}

uint8_t CompactFlash_SectorCount_Write(CompactFlash *device, uint8_t sectorCount) {
    return 0;
}

uint8_t CompactFlash_SectorNumber_Write(CompactFlash *device, uint8_t number, uint8_t offset) {
    return 0;
}

uint8_t CompactFlash_Command_ReadSector(CompactFlash *device) {
    return 0;
}

uint8_t CompactFlash_Read_Byte(CompactFlash *device) {
    return 0;
}

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
    HBC56Device testDevice = createCompactFlashDevice(0, NULL);

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

void test_readDevice_on_CF_STAT_statusIsReturned(void)
{
    // arrange
    test_reset();

    HBC56Device testDevice = createCompactFlashDevice(BASE_ADDRESS, NULL);
    uint8_t val = 0;
    compactFlashSpy.status = 0b10011001;

    // act
    readDevice(&testDevice, BASE_ADDRESS + CF_STAT, &val, 0);

    // assert
    TEST_ASSERT(val == compactFlashSpy.status);
}

TEST_LIST = {
   { "test_createDevice_nameIsSet", test_createDevice_nameIsSet },
   { "test_createDevice_cfCardIsCreated", test_createDevice_cfCardIsCreated },
   { "test_destroyDevice_cfCardIsDestroyed", test_destroyDevice_cfCardIsDestroyed },
   { "test_readDevice_statusIsReturned", test_readDevice_on_CF_STAT_statusIsReturned },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};