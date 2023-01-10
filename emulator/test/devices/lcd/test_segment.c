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

// void test_turnOn_turnsOnDevice(void)
// {
//     // arrange
//     LcdSegment* segment = LcdSegment_Create();

//     // act
//     LcdSegment_TurnOn(segment);

//     // assert
//     TEST_ASSERT(LcdSegment_State() == LCD_STATE_ON);
// }

// void test_turnOnTwice_turnsOnDevice(void)
// {
//     // arrange
//     LcdSegment* segment = LcdSegment_Create();

//     // act
//     LcdSegment_TurnOn(segment);
//     LcdSegment_TurnOn(segment);

//     // assert
//     TEST_ASSERT(LcdSegment_State() == LCD_STATE_ON);
// }

TEST_LIST = {
    { "test_createSegment", test_createSegment },
    { "test_destroySegment", test_destroySegment },
    { "test_state_turnedOffByDefault", test_state_turnedOffByDefault },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};