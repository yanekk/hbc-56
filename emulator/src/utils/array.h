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

#ifndef _HBC56_ARRAY_H_
#define _HBC56_ARRAY_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include <stdint.h>

void Array_ShiftRight(uint8_t* input, uint8_t size, uint8_t* output);

#ifdef __cplusplus
}
#endif

#endif