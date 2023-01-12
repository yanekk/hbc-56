/*
 * Troy's HBC-56 Emulator - Audio
 *
 * Copyright (c) 2021 Troy Schrapel
 *
 * This code is licensed under the MIT license
 *
 * https://github.com/visrealm/hbc-56/emulator
 *
 */

#ifndef _HBC56_MATRIX_H_
#define _HBC56_MATRIX_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t* data;
    uint8_t width, height;
} Matrix;

void Matrix_GetColumn(Matrix* matrix, uint8_t columnIndex, uint8_t* buffer);
void Matrix_SetColumn(Matrix* matrix, uint8_t columnIndex, uint8_t* data);

#ifdef __cplusplus
}
#endif

#endif