/*
 * Troy's HBC-56 Emulator - AY-3-8910 device
 *
 * Copyright (c) 2021 Troy Schrapel
 *
 * This code is licensed under the MIT license
 *
 * https://github.com/visrealm/hbc-56/emulator
 *
 */

#ifndef _HBC56_RENDERER_H_
#define _HBC56_RENDERER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void LcdRenderer_Render(uint32_t* displayData);

#ifdef __cplusplus
}
#endif


#endif