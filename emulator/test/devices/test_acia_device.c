#include "acutest.h"
#include "devices/acia_device.h"

#define BASE_ADDRESS 0x4800

void test_createDevice_nameIsSet(void)
{
    // act
    HBC56Device testDevice = createAciaDevice(BASE_ADDRESS);

    // assert 
    TEST_CHECK(strcmp(testDevice.name, "ACIA") == 0);

    // cleanup
}

void test_readDevice_returnsZeroOnAddressOutsideOfScope(void)
{
    // arrange
    HBC56Device testDevice = createAciaDevice(BASE_ADDRESS);
    
    // act
    uint8_t value = 0;
    uint8_t isHandled = readDevice(&testDevice, BASE_ADDRESS - 1, &value, true);
    TEST_CHECK(isHandled == 0);

    isHandled = readDevice(&testDevice, BASE_ADDRESS + 4, &value, true);
    TEST_CHECK(isHandled == 0);

    // assert
    TEST_CHECK(isHandled == 0);

    // cleanup
    destroyAciaDevice(&testDevice);
}

void test_readDevice_returnsOneOnAddressInsideScope(void)
{
    // arrange
    HBC56Device testDevice = createAciaDevice(BASE_ADDRESS);
    
    // act
    uint8_t value = 0;
    uint8_t isHandled = readDevice(&testDevice, BASE_ADDRESS, &value, true);
    TEST_CHECK(isHandled == 1);


    isHandled = readDevice(&testDevice, BASE_ADDRESS+3, &value, true);
    TEST_CHECK(isHandled == 1);

    // cleanup
    destroyAciaDevice(&testDevice);
}

void test_writeDevice_returnsZeroOnAddressOutsideOfScope(void)
{
    // arrange
    HBC56Device testDevice = createAciaDevice(BASE_ADDRESS);
    
    // act & assert
    TEST_CHECK(writeDevice(&testDevice, BASE_ADDRESS-1, 0) == 0);
    TEST_CHECK(writeDevice(&testDevice, BASE_ADDRESS+4, 0) == 0);

    // cleanup
    destroyAciaDevice(&testDevice);
}

void test_writeDevice_returnsOneOnAddressInsideScope(void)
{
    // arrange
    HBC56Device testDevice = createAciaDevice(BASE_ADDRESS);
    
    // act
    TEST_CHECK(writeDevice(&testDevice, BASE_ADDRESS, 0) == 1);
    TEST_CHECK(writeDevice(&testDevice, BASE_ADDRESS+3, 0) == 1);

    // cleanup
    destroyAciaDevice(&testDevice);
}

char buffer[64] = {0};
char* bufferPtr = buffer;

void printToBuffer(uint8_t c) {
    *bufferPtr++ = c;
}

void test_writeDevice_bytesAreTransmitted(void)
{
    // arrange
    buffer[0] = 0;
    bufferPtr = buffer;
    HBC56Device testDevice = createAciaDeviceWithOutput(BASE_ADDRESS, printToBuffer);
    
    // act
    writeDevice(&testDevice, BASE_ADDRESS, 'a');
    writeDevice(&testDevice, BASE_ADDRESS, 'b');
    writeDevice(&testDevice, BASE_ADDRESS, 'c');
    
    TEST_CHECK(strcmp(buffer, "abc") == 0);

    // cleanup
    destroyAciaDevice(&testDevice);
}

TEST_LIST = {
   { "test_createDevice_nameIsSet", test_createDevice_nameIsSet },
   { "test_readDevice_returnsZeroOnAddressOutsideOfScope", test_readDevice_returnsZeroOnAddressOutsideOfScope },
   { "test_readDevice_returnsOneOnAddressInsideScope", test_readDevice_returnsOneOnAddressInsideScope },
   { "test_writeDevice_returnsZeroOnAddressOutsideOfScope", test_writeDevice_returnsZeroOnAddressOutsideOfScope },
   { "test_writeDevice_returnsOneOnAddressInsideScope", test_writeDevice_returnsOneOnAddressInsideScope },
   { "test_writeDevice_bytesAreTransmitted", test_writeDevice_bytesAreTransmitted },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};