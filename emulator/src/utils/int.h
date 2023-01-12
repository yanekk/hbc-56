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

#ifndef _HBC56_INT_H_
#define _HBC56_INT_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include <stdint.h>
/// @brief 
/// @param input number to operate on
/// @param carry from previous operation
/// @param output end result
/// @return carry  
bool Int_ShiftRightWithCarry(uint8_t input, bool carry, uint8_t* output);

#ifdef __cplusplus
}
#endif

#endif