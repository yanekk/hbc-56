#include <stdint.h>
#include <stdbool.h>

#include "acutest.h"
#include "utils/array.h"

void test_shiftRightMultiple_withCarry() {
    // arrange
    uint8_t initial[2] = { 0b00000111, 0b00001111 };
    uint8_t actual[2] = {0};

    // act
    Array_ShiftRight((uint8_t*)initial, 2, (uint8_t*)actual);

    // assert
    TEST_ASSERT(actual[0] == 0b10000011 && actual[1] == 0b10000111);
}

void test_shiftRightMultiple_withoutCarry() {
    // arrange
    uint8_t initial[2] = { 0b00000111, 0b00001110 };
    uint8_t actual[2] = {0};

    // act
    Array_ShiftRight((uint8_t*)initial, 2, (uint8_t*)actual);

    // assert
    TEST_ASSERT(actual[0] == 0b00000011 && actual[1] == 0b10000111);
}

#define BYTES_COUNT 2
void test_shiftRightWholeTwoBytes() {
    // arrange
    uint8_t initial[BYTES_COUNT] = { 0xFF, 0x00 };
    uint8_t results[32];

    // act
    uint8_t current[BYTES_COUNT] = {0};

    for(uint8_t j = 0; j < 16; j++) {
        Array_ShiftRight((uint8_t*)initial, BYTES_COUNT, (uint8_t*)current);
        for (uint8_t i = 0; i < BYTES_COUNT; i++) {
            results[j * BYTES_COUNT + i] = current[i];
        }
        memcpy(initial, current, BYTES_COUNT);
    }
    TEST_ASSERT(results[0] == 0x7f && results[1] == 0x80);
    TEST_ASSERT(results[2] == 0x3f && results[3] == 0xC0);
    TEST_ASSERT(results[4] == 0x1f && results[5] == 0xE0);
    TEST_ASSERT(results[6] == 0x0f && results[7] == 0xF0);
    TEST_ASSERT(results[8] == 0x07 && results[9] == 0xF8);
    TEST_ASSERT(results[10] == 0x03 && results[11] == 0xFC);
    TEST_ASSERT(results[12] == 0x01 && results[13] == 0xFE);
    TEST_ASSERT(results[14] == 0x00 && results[15] == 0xFF);
    TEST_ASSERT(results[16] == 0x80 && results[17] == 0x7F);
    TEST_ASSERT(results[18] == 0xc0 && results[19] == 0x3F);
    TEST_ASSERT(results[20] == 0xe0 && results[21] == 0x1F);
    TEST_ASSERT(results[22] == 0xf0 && results[23] == 0x0F);
    TEST_ASSERT(results[24] == 0xf8 && results[25] == 0x07);
    TEST_ASSERT(results[26] == 0xfc && results[27] == 0x03);
    TEST_ASSERT(results[28] == 0xfe && results[29] == 0x01);
    TEST_ASSERT(results[30] == 0xff && results[31] == 0x00);
}

TEST_LIST = {
    { "test_shiftRightMultiple_withCarry", test_shiftRightMultiple_withCarry },
    { "test_shiftRightMultiple_withoutCarry", test_shiftRightMultiple_withoutCarry },
    { "test_shiftRightWholeTwoBytes", test_shiftRightWholeTwoBytes },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};
