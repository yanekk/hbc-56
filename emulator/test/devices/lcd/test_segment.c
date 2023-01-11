#include "acutest.h"
#include "devices/lcd/segment.h"

#include <stdbool.h>
#include <string.h>

void test_createSegment(void)
{
    // act
    LcdSegment* segment = LcdSegment_Create();

    // assert
    TEST_ASSERT(segment != NULL);
}

void test_destroySegment(void)
{
    // arrange
    LcdSegment* segment = LcdSegment_Create();

    // act
    bool ok = LcdSegment_Destroy(segment);

    // assert
    TEST_ASSERT(ok);
}

void test_state_turnedOffByDefault(void)
{
    // arrange
    LcdSegment* segment = LcdSegment_Create();

    // assert
    TEST_ASSERT(LcdSegment_State(segment) == LCD_STATE_OFF);
}

void test_turnOn_turnsOnDevice(void)
{
    // arrange
    LcdSegment* segment = LcdSegment_Create();

    // act
    LcdSegment_TurnOn(segment);

    // assert
    TEST_ASSERT(LcdSegment_State(segment) == LCD_STATE_ON);
}

void test_turnOnTwice_keepDeviceTurnedOn(void)
{
    // arrange
    LcdSegment* segment = LcdSegment_Create();

    // act
    LcdSegment_TurnOn(segment);
    LcdSegment_TurnOn(segment);

    // assert
    TEST_ASSERT(LcdSegment_State(segment) == LCD_STATE_ON);
}

void test_turnOffOnNewSegment_keepSegmentTurnedOff(void)
{
    // arrange
    LcdSegment* segment = LcdSegment_Create();

    // act
    LcdSegment_TurnOff(segment);

    // assert
    TEST_ASSERT(LcdSegment_State(segment) == LCD_STATE_OFF);
}

void test_turnOff_turnsOffSegment(void)
{
    // arrange
    LcdSegment* segment = LcdSegment_Create();
    LcdSegment_TurnOn(segment);

    // act
    LcdSegment_TurnOff(segment);

    // assert
    TEST_ASSERT(LcdSegment_State(segment) == LCD_STATE_OFF);
}

void test_turnOffTwice_keepDeviceTurnedOff(void)
{
    // arrange
    LcdSegment* segment = LcdSegment_Create();
    LcdSegment_TurnOn(segment);

    // act
    LcdSegment_TurnOff(segment);
    LcdSegment_TurnOff(segment);

    // assert
    TEST_ASSERT(LcdSegment_State(segment) == LCD_STATE_OFF);
}

void test_storesByteAtDefaultAddress(void)
{
    // arrange
    LcdSegment* segment = LcdSegment_Create();

    // act
    LcdSegment_WriteData(segment, 53);
    LcdSegment_SetAddress(segment, 0);

    // assert
    TEST_ASSERT(LcdSegment_ReadData(segment, false) == 53);
}

void test_storingAndReadingByteIncreasesAddress(void)
{
    // arrange
    LcdSegment* segment = LcdSegment_Create();

    // act
    LcdSegment_WriteData(segment, 53);
    LcdSegment_WriteData(segment, 35);
    LcdSegment_SetAddress(segment, 0);

    // assert
    TEST_ASSERT(LcdSegment_ReadData(segment, false) == 53);
    TEST_ASSERT(LcdSegment_ReadData(segment, false) == 35);
}

void test_writeData_addressOverflow(void)
{
    // arrange
    LcdSegment* segment = LcdSegment_Create();

    // act
    LcdSegment_WriteData(segment, 53);
    LcdSegment_SetAddress(segment, LCD_SEGMENT_COLUMNS-1);
    LcdSegment_WriteData(segment, 35);

    // assert
    TEST_ASSERT(LcdSegment_ReadData(segment, false) == 53);
}

void test_readData_addressOverflow(void)
{
    // arrange
    LcdSegment* segment = LcdSegment_Create();

    // act
    LcdSegment_WriteData(segment, 53);
    LcdSegment_SetAddress(segment, LCD_SEGMENT_COLUMNS-1);
    LcdSegment_ReadData(segment, false);

    // assert
    TEST_ASSERT(LcdSegment_ReadData(segment, false) == 53);
}

void test_readData_debugFlagNoAddressIncrease(void)
{
    // arrange
    LcdSegment* segment = LcdSegment_Create();

    // act
    LcdSegment_WriteData(segment, 1);
    LcdSegment_WriteData(segment, 2);
    
    LcdSegment_SetAddress(segment, 0);

    // assert
    TEST_ASSERT(LcdSegment_ReadData(segment, true) == 1 
        && LcdSegment_ReadData(segment, true) == 1);
}

void test_writeBuffer(void)
{
    // arrange
    LcdSegment* segment = LcdSegment_Create();
    LcdSegment_WriteData(segment, 1);
    LcdSegment_WriteData(segment, 2);
    LcdSegment_WriteData(segment, 4);
    
    LcdSegment_SetAddress(segment, LCD_SEGMENT_COLUMNS-3);

    LcdSegment_WriteData(segment, 6);
    LcdSegment_WriteData(segment, 8);
    LcdSegment_WriteData(segment, 10);


    uint8_t buffer[LCD_SEGMENT_COLUMNS * LCD_SEGMENT_ROWS] = {0};

    // act
    LcdSegment_CopyVram(segment, (uint8_t*)&buffer);

    // assert
    TEST_ASSERT(buffer[0] == 1);
    TEST_ASSERT(buffer[1] == 2);
    TEST_ASSERT(buffer[2] == 4);
    TEST_ASSERT(buffer[3] == 0);

    TEST_ASSERT(buffer[LCD_SEGMENT_COLUMNS - 3] == 6);
    TEST_ASSERT(buffer[LCD_SEGMENT_COLUMNS - 2] == 8);
    TEST_ASSERT(buffer[LCD_SEGMENT_COLUMNS - 1] == 10);
}

TEST_LIST = {
    { "test_createSegment", test_createSegment },
    { "test_destroySegment", test_destroySegment },
    { "test_turnOn_turnsOnDevice", test_turnOn_turnsOnDevice },
    { "test_turnOnTwice_keepDeviceTurnedOn", test_turnOnTwice_keepDeviceTurnedOn },
    { "test_turnOffOnNewSegment_keepSegmentTurnedOff", test_turnOffOnNewSegment_keepSegmentTurnedOff },
    { "test_turnOff_turnsOffSegment", test_turnOff_turnsOffSegment },
    { "test_turnOffTwice_keepDeviceTurnedOff", test_turnOffTwice_keepDeviceTurnedOff },
    { "test_storesByteAtDefaultAddress", test_storesByteAtDefaultAddress },
    { "test_storingAndReadingByteIncreasesAddress", test_storingAndReadingByteIncreasesAddress },
    { "test_writeData_addressOverflow", test_writeData_addressOverflow },
    { "test_readData_addressOverflow", test_readData_addressOverflow },
    { "test_readData_debugFlagNoAddressIncrease", test_readData_debugFlagNoAddressIncrease },
    { "test_writeBuffer", test_writeBuffer },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};