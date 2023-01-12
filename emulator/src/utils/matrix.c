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