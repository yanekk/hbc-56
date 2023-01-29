#include "acutest.h"
#include "devices/via_device.h"

#define BASE_ADDRESS 0x6000

void test_createDevice_nameIsSet(void)
{
    // act
    HBC56Device testDevice = createViaDevice(BASE_ADDRESS);

    // assert 
    TEST_CHECK(strcmp(testDevice.name, "VIA") == 0);

    // cleanup
}

void test_readDevice_returnsZeroOnAddressOutsideOfScope(void)
{
    // arrange
    HBC56Device testDevice = createViaDevice(BASE_ADDRESS);
    
    // act
    uint8_t value = 0;
    uint8_t isHandled = readDevice(&testDevice, BASE_ADDRESS - 1, &value, true);
    TEST_CHECK(isHandled == 0);

    isHandled = readDevice(&testDevice, BASE_ADDRESS + 15, &value, true);
    TEST_CHECK(isHandled == 0);

    // assert
    TEST_CHECK(isHandled == 0);

    // cleanup
    destroyDevice(&testDevice);
}

void test_readDevice_returnsOneOnAddressInsideScope(void)
{
    // arrange
    HBC56Device testDevice = createViaDevice(BASE_ADDRESS);
    
    // act
    uint8_t value = 0;
    uint8_t isHandled = readDevice(&testDevice, BASE_ADDRESS, &value, true);
    TEST_CHECK(isHandled == 1);


    isHandled = readDevice(&testDevice, BASE_ADDRESS+3, &value, true);
    TEST_CHECK(isHandled == 1);

    // cleanup
    destroyDevice(&testDevice);
}

void test_writeDevice_returnsZeroOnAddressOutsideOfScope(void)
{
    // arrange
    HBC56Device testDevice = createViaDevice(BASE_ADDRESS);
    
    // act & assert
    TEST_CHECK(writeDevice(&testDevice, BASE_ADDRESS-1, 0) == 0);
    TEST_CHECK(writeDevice(&testDevice, BASE_ADDRESS+15, 0) == 0);

    // cleanup
    destroyDevice(&testDevice);
}

void test_writeDevice_returnsOneOnAddressInsideScope(void)
{
    // arrange
    HBC56Device testDevice = createViaDevice(BASE_ADDRESS);
    
    // act
    TEST_CHECK(writeDevice(&testDevice, BASE_ADDRESS, 0) == 1);
    TEST_CHECK(writeDevice(&testDevice, BASE_ADDRESS+3, 0) == 1);

    // cleanup
    destroyDevice(&testDevice);
}

TEST_LIST = {
   { "test_createDevice_nameIsSet", test_createDevice_nameIsSet },
   { "test_readDevice_returnsZeroOnAddressOutsideOfScope", test_readDevice_returnsZeroOnAddressOutsideOfScope },
   { "test_readDevice_returnsOneOnAddressInsideScope", test_readDevice_returnsOneOnAddressInsideScope },
   { "test_writeDevice_returnsZeroOnAddressOutsideOfScope", test_writeDevice_returnsZeroOnAddressOutsideOfScope },
   { "test_writeDevice_returnsOneOnAddressInsideScope", test_writeDevice_returnsOneOnAddressInsideScope },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};