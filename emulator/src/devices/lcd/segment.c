#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "segment.h"

struct LcdSegment {
    LcdState state;
    uint8_t* vram;
    uint8_t y;
    uint8_t x;
};

LcdSegment* LcdSegment_Create() {
    LcdSegment* segment = malloc(sizeof(LcdSegment));
    segment->state = LCD_STATE_OFF;
    segment->y = 0;
    segment->x = 0;
    segment->vram = calloc(sizeof(uint8_t),  LCD_SEGMENT_SIZE);
    return segment;
}

bool LcdSegment_Destroy(LcdSegment* segment) {
    if(segment != NULL) {
        free(segment);
    }
    return true;
}

LcdState LcdSegment_State(LcdSegment * segment) {
    return segment->state;
}

void LcdSegment_TurnOn(LcdSegment * segment) {
    segment->state = LCD_STATE_ON;
}

void LcdSegment_TurnOff(LcdSegment * segment) {
    segment->state = LCD_STATE_OFF;
}

uint8_t LcdSegment_ReadData(LcdSegment* segment, bool debug) {
    uint8_t data = segment->vram[segment->y*LCD_SEGMENT_COLUMNS + segment->x];
    if(debug)
        return data;
        
    segment->y++;
    if(segment->y == 64)
        segment->y = 0;
    return data;
}

void LcdSegment_WriteData(LcdSegment* segment, uint8_t data) {
    segment->vram[segment->y*LCD_SEGMENT_COLUMNS + segment->x] = data;
    segment->y++;
    if(segment->y == 64)
        segment->y = 0;
}

void LcdSegment_SetAddress(LcdSegment* segment, uint8_t y) {
    segment->y = y;
}

void LcdSegment_CopyVram(LcdSegment* segment, uint8_t* buffer) {
    memcpy(buffer, segment->vram, sizeof(uint8_t) * LCD_SEGMENT_SIZE);
}