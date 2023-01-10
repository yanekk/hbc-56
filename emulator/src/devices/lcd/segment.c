#include <stdlib.h>
#include <stdbool.h>

#include "segment.h"

struct LcdSegment {
    LcdState state;
};

LcdSegment* LcdSegment_Create() {
    LcdSegment* segment = malloc(sizeof(LcdSegment));
    segment->state = LCD_STATE_OFF;
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