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
    uint8_t buffer[LCD_SEGMENT_COLUMNS * LCD_SEGMENT_ROWS] = {0};
    LcdSegment_CopyVram(segment, (uint8_t*)&buffer);
    
    TEST_ASSERT(buffer[0] == 53);
}

void test_writingByteIncreasesAddress(void)
{
    // arrange
    LcdSegment* segment = LcdSegment_Create();

    // act
    LcdSegment_WriteData(segment, 53);
    LcdSegment_WriteData(segment, 35);

    // assert
    uint8_t buffer[LCD_SEGMENT_COLUMNS * LCD_SEGMENT_ROWS] = {0};
    LcdSegment_CopyVram(segment, (uint8_t*)&buffer);

    TEST_ASSERT(buffer[0] == 53);
    TEST_ASSERT(buffer[1] == 35);
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

void test_copyVram(void)
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

void test_setPage_storesDataAtRightPosition(void)
{
    // arrange
    LcdSegment* segment = LcdSegment_Create();

    // act
    for(size_t x = 0; x < 8; x++) {
        LcdSegment_SetPage(segment, x);
        LcdSegment_SetAddress(segment, 0);
        
        LcdSegment_WriteData(segment, 1 + x);
        LcdSegment_WriteData(segment, 2 + x);
        LcdSegment_WriteData(segment, 3 + x);

        LcdSegment_SetAddress(segment, LCD_SEGMENT_COLUMNS-3);

        LcdSegment_WriteData(segment, 4 + x);
        LcdSegment_WriteData(segment, 5 + x);
        LcdSegment_WriteData(segment, 6 + x);
    }

    // assert
    uint8_t buffer[LCD_SEGMENT_COLUMNS * LCD_SEGMENT_ROWS] = {0};
    LcdSegment_CopyVram(segment, (uint8_t*)&buffer);
    for(size_t x = 0; x < 8; x++) {
        TEST_ASSERT(buffer[x * LCD_SEGMENT_COLUMNS + 0] == 1+x);
        TEST_ASSERT(buffer[x * LCD_SEGMENT_COLUMNS + 1] == 2+x);
        TEST_ASSERT(buffer[x * LCD_SEGMENT_COLUMNS + 2] == 3+x);

        TEST_ASSERT(buffer[x * LCD_SEGMENT_COLUMNS + LCD_SEGMENT_COLUMNS - 3] == 4+x);
        TEST_ASSERT(buffer[x * LCD_SEGMENT_COLUMNS + LCD_SEGMENT_COLUMNS - 2] == 5+x);
        TEST_ASSERT(buffer[x * LCD_SEGMENT_COLUMNS + LCD_SEGMENT_COLUMNS - 1] == 6+x);
    }
}

// void test_setStartLine_pushesDataForwardWithinOneByte(void)
// {
//     // arrange
//     LcdSegment* segment = LcdSegment_Create();
//     uint8_t buffer[LCD_SEGMENT_COLUMNS * LCD_SEGMENT_ROWS] = {0};
//     uint8_t data[8][8] = {
//         {12, 23, 34, 45, 56, 67, 78, 89},
//         {89, 12, 23, 34, 45, 56, 67, 78},
//         {78, 89, 12, 23, 34, 45, 56, 67},
//         {67, 78, 89, 12, 23, 34, 45, 56},
//         {56, 67, 78, 89, 12, 23, 34, 45},
//         {45, 56, 67, 78, 89, 12, 23, 34},
//         {34, 45, 56, 67, 78, 89, 12, 23},
//         {23, 34, 45, 56, 67, 78, 89, 12},
//     };
    
//     for (uint8_t x = 0; x < 8; x++) {
//         LcdSegment_SetPage(segment, x);
//         LcdSegment_SetAddress(segment, 0);
//         LcdSegment_WriteData(segment, data[0][x]);
//     }

//     for (uint8_t startLine8 = 0; startLine8 < 8; startLine8++) {
//         // act
//         LcdSegment_SetStartLine(segment, 8 * startLine8);

//         // assert
//         LcdSegment_CopyVram(segment, (uint8_t*)&buffer);
//         for(uint8_t page = 0; page < 8; page++) {
//             TEST_ASSERT(buffer[page * LCD_SEGMENT_COLUMNS] == data[startLine8][page]);
//         }
//     }
// }

TEST_LIST = {
    { "test_createSegment", test_createSegment },
    { "test_destroySegment", test_destroySegment },
    { "test_turnOn_turnsOnDevice", test_turnOn_turnsOnDevice },
    { "test_turnOnTwice_keepDeviceTurnedOn", test_turnOnTwice_keepDeviceTurnedOn },
    { "test_turnOffOnNewSegment_keepSegmentTurnedOff", test_turnOffOnNewSegment_keepSegmentTurnedOff },
    { "test_turnOff_turnsOffSegment", test_turnOff_turnsOffSegment },
    { "test_turnOffTwice_keepDeviceTurnedOff", test_turnOffTwice_keepDeviceTurnedOff },
    { "test_storesByteAtDefaultAddress", test_storesByteAtDefaultAddress },
    { "test_writingByteIncreasesAddress", test_writingByteIncreasesAddress },
    { "test_writeData_addressOverflow", test_writeData_addressOverflow },
    { "test_readData_addressOverflow", test_readData_addressOverflow },
    { "test_readData_debugFlagNoAddressIncrease", test_readData_debugFlagNoAddressIncrease },
    { "test_copyVram", test_copyVram },
    { "test_setPage_storesDataAtRightPosition", test_setPage_storesDataAtRightPosition },
    // { "test_setStartLine_pushesDataForwardWithinOneByte", test_setStartLine_pushesDataForwardWithinOneByte },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};
