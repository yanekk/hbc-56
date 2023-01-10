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

TEST_LIST = {
    { "test_createSegment", test_createSegment },
    { "test_destroySegment", test_destroySegment },
    { "test_turnOn_turnsOnDevice", test_turnOn_turnsOnDevice },
    { "test_turnOnTwice_keepDeviceTurnedOn", test_turnOnTwice_keepDeviceTurnedOn },
    { "test_turnOffOnNewSegment_keepSegmentTurnedOff", test_turnOffOnNewSegment_keepSegmentTurnedOff },
    { "test_turnOff_turnsOffSegment", test_turnOff_turnsOffSegment },
    { "test_turnOffTwice_keepDeviceTurnedOff", test_turnOffTwice_keepDeviceTurnedOff },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};