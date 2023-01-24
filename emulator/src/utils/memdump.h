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

#ifndef _HBC56_MEMDUMP_H_
#define _HBC56_MEMDUMP_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void MemDump_Save(uint8_t (readMemory)(uint16_t, bool));

#ifdef __cplusplus
}
#endif

#endif