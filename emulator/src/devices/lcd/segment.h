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

#ifndef _HBC56_SEGMENT_H_
#define _HBC56_SEGMENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    LCD_STATE_OFF,
    LCD_STATE_ON
} LcdState;

struct LcdSegment;
typedef struct LcdSegment LcdSegment;

LcdSegment* LcdSegment_Create();

bool LcdSegment_Destroy(LcdSegment * device);

LcdState LcdSegment_State(LcdSegment * device);

#ifdef __cplusplus
}
#endif


#endif