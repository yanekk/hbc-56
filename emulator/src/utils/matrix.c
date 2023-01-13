#include "matrix.h"

void Matrix_GetColumn(Matrix* matrix, uint8_t columnIndex, uint8_t* buffer) {
    for(uint8_t row = 0; row < matrix->height; row++) {
        buffer[row] = matrix->data[row * matrix->width + columnIndex];
    }
}

void Matrix_SetColumn(Matrix* matrix, uint8_t columnIndex, uint8_t* data) {
    for(uint8_t row = 0; row < matrix->height; row++) {
        matrix->data[row * matrix->width + columnIndex] = data[row];
    }
}

void Matrix_MergeToBitArray(Matrix* matrixA, Matrix* matrixB, uint32_t* bitArray) {
    uint8_t byteWidth = matrixA->width + matrixB->width;
    uint8_t byteHeight = matrixA->height;
    Matrix* currentMatrix;
    uint8_t address;

    for(uint8_t x = 0; x < byteHeight; x++) {
        for(uint8_t bit = 0; bit < 8; bit++){
            for(uint8_t y = 0; y < byteWidth; y++) {
                currentMatrix = y < matrixA->width ? matrixA : matrixB;
                address = y < matrixA->width ? y : y - matrixA->width;
                
                uint16_t bitAddress = (x*8*byteWidth) + (bit * byteWidth) + y;
                uint32_t value = currentMatrix->data[x * currentMatrix->width + address];

                bitArray[bitAddress] = (value >> (7-bit)) & 0x01;
            }
        }
    }
}