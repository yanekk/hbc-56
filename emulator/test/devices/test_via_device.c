#include "acutest.h"
#include "devices/via_device.h"

#define BASE_ADDRESS 0x6000
#define VIA_REG(reg) (BASE_ADDRESS+reg)

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
    uint8_t isHandled = readDevice(&testDevice, VIA_REG(VIA_PB) - 1, &value, true);
    TEST_CHECK(isHandled == 0);

    isHandled = readDevice(&testDevice, VIA_REG(VIA_IER) + 1, &value, true);
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
    uint8_t isHandled = readDevice(&testDevice, VIA_REG(VIA_PB), &value, true);
    TEST_CHECK(isHandled == 1);


    isHandled = readDevice(&testDevice, VIA_REG(VIA_DDRA), &value, true);
    TEST_CHECK(isHandled == 1);

    // cleanup
    destroyDevice(&testDevice);
}

void test_writeDevice_returnsZeroOnAddressOutsideOfScope(void)
{
    // arrange
    HBC56Device testDevice = createViaDevice(BASE_ADDRESS);
    
    // act & assert
    TEST_CHECK(writeDevice(&testDevice, VIA_REG(VIA_PB) - 1, 0) == 0);
    TEST_CHECK(writeDevice(&testDevice, VIA_REG(VIA_IER) + 1, 0) == 0);

    // cleanup
    destroyDevice(&testDevice);
}

void test_writeDevice_returnsOneOnAddressInsideScope(void)
{
    // arrange
    HBC56Device testDevice = createViaDevice(BASE_ADDRESS);
    
    // act
    TEST_CHECK(writeDevice(&testDevice, VIA_REG(VIA_PB), 0) == 1);
    TEST_CHECK(writeDevice(&testDevice, BASE_ADDRESS+3, 0) == 1);

    // cleanup
    destroyDevice(&testDevice);
}

uint8_t hbcInterruptCallCount = 0;
void test_timer1_freeMode_noPB7Output(void)
{
    // arrange
    uint8_t timerCycles = 16;
    HBC56Device testDevice = createViaDevice(BASE_ADDRESS);
    
    // act
    writeDevice(&testDevice, VIA_REG(VIA_ACR), VIA_ACR_TIMER1_CONTINOUS_MODE);
    // enable continous mode with PB7 disabled

    writeDevice(&testDevice, VIA_REG(VIA_IER), VIA_IER_SET | VIA_IER_TIMER1);
    // enable Timer1 interrupt

    writeDevice(&testDevice, VIA_REG(VIA_T1CL), timerCycles);
    writeDevice(&testDevice, VIA_REG(VIA_T1CH), 0);
    // counting starts on writing to VIA_T1CH

    // cycle to reach the interrupt
    tickDevice(&testDevice, 16, 0);

    // assert
    TEST_CHECK_(hbcInterruptCallCount == 1, "expected HBC interrupt to be called once, found %d", hbcInterruptCallCount);

    uint8_t actualInterruptFlag;
    readDevice(&readDevice, VIA_REG(VIA_IFR), &actualInterruptFlag, true);
    TEST_CHECK_(actualInterruptFlag == VIA_IFR_ANY | VIA_IFR_TIMER1, 
        "expected interrupt flag to be set to timer1, found %X", actualInterruptFlag);

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