#include <stdint.h>
#include <stdbool.h>

#include "acutest.h"
#include "utils/int.h"

void test_shiftRightWithCarry_withCarry() {
    // arrange
    uint8_t input = 0b11110000;
    uint8_t output = 0;

    // act
    Int_ShiftRightWithCarry(input, true, &output);

    // assert
    TEST_ASSERT(output == 0b11111000);
}

void test_shiftRightWithCarry_withoutCarry() {
    // arrange
    uint8_t input = 0b11110000;
    uint8_t output = 0;

    // act
    Int_ShiftRightWithCarry(input, false, &output);

    // assert
    TEST_ASSERT(output == 0b01111000);
}

void test_shiftRightWithCarry_returnsCarry() {
    // arrange
    uint8_t input = 0b11110001;
    uint8_t output = 0;

    // act
    bool carry = Int_ShiftRightWithCarry(input, false, &output);

    // assert
    TEST_ASSERT(carry == true);
}

void test_shiftRightWithCarry_notReturnCarry() {
    // arrange
    uint8_t input = 0b11110000;
    uint8_t output = 0;

    // act
    bool carry = Int_ShiftRightWithCarry(input, false, &output);

    // assert
    TEST_ASSERT(carry == false);
}

TEST_LIST = {
    { "test_shiftRightWithCarry_withCarry", test_shiftRightWithCarry_withCarry },
    { "test_shiftRightWithCarry_withoutCarry", test_shiftRightWithCarry_withoutCarry },
    { "test_shiftRightWithCarry_returnsCarry", test_shiftRightWithCarry_returnsCarry },
    { "test_shiftRightWithCarry_notReturnCarry", test_shiftRightWithCarry_notReturnCarry },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};
