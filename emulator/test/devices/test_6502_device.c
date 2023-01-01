#include "acutest.h"
#include "devices/6502_device.h"
#include "devices/device.h"

uint8_t hbc56MemRead(uint16_t addr, bool dbg) {
    return 0;
}

void hbc56MemWrite(uint16_t addr, uint8_t val) {

}

void test_createDevice_nameIsSet(void)
{
    const HBC56Device testDevice = create6502CpuDevice(NULL);
    TEST_ASSERT(strcmp(testDevice.name, "6502 CPU") == 0);
}

void test_createDevice_initializesCpuDevice(void)
{
    const HBC56Device testDevice = create6502CpuDevice(NULL);
    TEST_ASSERT(testDevice.data != NULL);
}

void test_tickDevice_(void)
{
    HBC56Device testDevice = create6502CpuDevice(NULL);
    tickDevice(&testDevice, 0, 0);

    TEST_ASSERT(testDevice.resetFn != NULL);
}

TEST_LIST = {
   { "test_createDevice_nameIsSet", test_createDevice_nameIsSet },
//    { "test_createDevice_initializesCpuDevice", test_createDevice_initializesCpuDevice },
//    { "test_tickDevice_", test_tickDevice_ },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};