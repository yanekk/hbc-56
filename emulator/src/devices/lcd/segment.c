#include <stdlib.h>
#include <stdbool.h>

#include "segment.h"

struct LcdSegment {
    LcdState state;
    uint8_t* vram;
};

LcdSegment* LcdSegment_Create() {
    LcdSegment* segment = malloc(sizeof(LcdSegment));
    segment->state = LCD_STATE_OFF;
    segment->vram = calloc(sizeof(uint8_t),  LCD_SEGMENT_COLUMNS * LCD_SEGMENT_ROWS);
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

uint8_t* LcdSegment_GetVRAM(LcdSegment* segment) {
    return segment->vram;
}