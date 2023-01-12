#include "acutest.h"
#include "utils/matrix.h"

void test_getColumn() {
    // arrange
    uint8_t matrixData[6] = {
        11, 22,
        12, 23,
        13, 24,
    };
    Matrix matrix = {
        .data = (uint8_t*)matrixData,
        .height = 3,
        .width = 2
    };

    uint8_t expected[2][3] = {
        { 11, 12, 13 },
        { 22, 23, 24 },
    };
    uint8_t buffer[3];

    // act
    for(uint8_t column = 0; column < 2; column++) {
        Matrix_GetColumn(&matrix, column, buffer);
        for(uint8_t i = 0; i < 2; i++){
            // assert
            TEST_ASSERT(buffer[i] == expected[column][i]);
        }
    }
}

void test_setColumn() {
    // arrange
    uint8_t matrixData[6] = {
        0x01, 0x04,
        0x02, 0x05,
        0x03, 0x06,
    };
    Matrix matrix = {
        .data = (uint8_t*)matrixData,
        .height = 3,
        .width = 2
    };

    uint8_t newData[3] = {
        0x0F,
        0x0A,
        0x0B,
    };

    // act
    Matrix_SetColumn(&matrix, 0, newData);

    // assert
    uint8_t buffer[3];
    Matrix_GetColumn(&matrix, 0, buffer);
    
    TEST_ASSERT(buffer[0] == newData[0]);
    TEST_ASSERT(buffer[1] == newData[1]);
    TEST_ASSERT(buffer[2] == newData[2]);
}

TEST_LIST = {
    { "test_getColumn", test_getColumn },
    { "test_setColumn", test_setColumn },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};
