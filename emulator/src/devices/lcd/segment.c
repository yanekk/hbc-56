#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "segment.h"

struct LcdSegment {
    LcdState state;
    uint8_t* vram;
    uint8_t y, x, startLine;
};

LcdSegment* LcdSegment_Create() {
    LcdSegment* segment = malloc(sizeof(LcdSegment));
    segment->state = LCD_STATE_OFF;
    segment->y = 0;
    segment->x = 0;
    segment->startLine = 0;
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
    uint8_t data = segment->vram[segment->x*LCD_SEGMENT_COLUMNS + segment->y];
    if(debug)
        return data;
        
    segment->y++;
    if(segment->y == 64)
        segment->y = 0;
    return data;
}

void LcdSegment_WriteData(LcdSegment* segment, uint8_t data) {
    segment->vram[segment->x*LCD_SEGMENT_COLUMNS + segment->y] = data;
    segment->y++;
    if(segment->y == 64)
        segment->y = 0;
}

void LcdSegment_SetAddress(LcdSegment* segment, uint8_t y) {
    segment->y = y;
}

void LcdSegment_SetPage(LcdSegment* segment, uint8_t page) {
    segment->x = page;
}

void LcdSegment_SetStartLine(LcdSegment* segment, uint8_t startLine) {
    segment->startLine = startLine;
}

void LcdSegment_CopyVram(LcdSegment* segment, uint8_t* buffer) {
    for(size_t x = 0; x < LCD_SEGMENT_ROWS; x++) {
        for(size_t y = 0; y < LCD_SEGMENT_COLUMNS; y++) {
            buffer[x * LCD_SEGMENT_COLUMNS + y] = segment->vram[x * LCD_SEGMENT_COLUMNS + y];
        }
    }
}