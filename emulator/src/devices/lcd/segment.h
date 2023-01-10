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

#define LCD_SEGMENT_COLUMNS 64
#define LCD_SEGMENT_ROWS 8

typedef enum {
    LCD_STATE_OFF,
    LCD_STATE_ON
} LcdState;

struct LcdSegment;
typedef struct LcdSegment LcdSegment;

LcdSegment* LcdSegment_Create();

bool LcdSegment_Destroy(LcdSegment* segment);

LcdState LcdSegment_State(LcdSegment* segment);

void LcdSegment_TurnOn(LcdSegment* segment);

void LcdSegment_TurnOff(LcdSegment* segment);

uint8_t* LcdSegment_GetVRAM(LcdSegment* segment);

#ifdef __cplusplus
}
#endif


#endif