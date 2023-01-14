#include "acutest.h"
#include "devices/dual_lcd_device.h"
#include "devices/device.h"

void test_createDevice_nameIsSet(void)
{
    const HBC56Device testDevice = createDualLcdDevice();
    TEST_ASSERT(strcmp(testDevice.name, "Dual Graphics LCD") == 0);
}

TEST_LIST = {
   { "test_createDevice_nameIsSet", test_createDevice_nameIsSet },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};