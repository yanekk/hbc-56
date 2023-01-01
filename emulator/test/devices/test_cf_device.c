#include "acutest.h"
#include "devices/cf_device.h"
#include "devices/device.h"

#define BASE_ADDRESS 0x1000

void test_createDevice_nameIsSet(void)
{
    const HBC56Device testDevice = createCompactFlashDevice(0, NULL);
    TEST_ASSERT(strcmp(testDevice.name, "CompactFlash") == 0);
}

void test_readDevice_statusIsReturned(void)
{
    // arrange
    const HBC56Device testDevice = createCompactFlashDevice(BASE_ADDRESS, NULL);
    const uint8_t val = 0;

    // act
    readDevice(&testDevice, BASE_ADDRESS + CF_STAT, &val, 0);

    // assert
    TEST_ASSERT(val == 0b01000100);
}

TEST_LIST = {
   { "test_createDevice_nameIsSet", test_createDevice_nameIsSet },
   { "test_readDevice_statusIsReturned", test_readDevice_statusIsReturned },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};