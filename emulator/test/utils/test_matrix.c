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

void test_mergeToBitArray_2x2() {
    // arrange
    uint8_t dataA[2] = {
        0b00111010,
        0b10100011
    };
    Matrix matrixA = {
        .height = 2,
        .width  = 1,
        .data   = dataA
    };

    uint8_t dataB[2] = {
        0b10101010,
        0b01010101
    };
    Matrix matrixB = {
        .height = 2,
        .width  = 1,
        .data   = dataB
    };

    uint32_t expectedBitArray[32] = {
        0, 1, 
        0, 0, 
        1, 1, 
        1, 0, 
        1, 1, 
        0, 0, 
        1, 1, 
        0, 0, 
        1, 0, 
        0, 1,
        1, 0, 
        0, 1,
        0, 0, 
        0, 1, 
        1, 0, 
        1, 1,
    };

    // act
    uint32_t actualBitArray[64];
    Matrix_MergeToBitArray(&matrixA, &matrixB, actualBitArray);

    // assert
    for (size_t i = 0; i < 32; i++)
    {   
        TEST_ASSERT(actualBitArray[i] == expectedBitArray[i]);
    }
}

TEST_LIST = {
    { "test_getColumn", test_getColumn },
    { "test_setColumn", test_setColumn },
    { "test_mergeToBitArray_2x2", test_mergeToBitArray_2x2 },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};
