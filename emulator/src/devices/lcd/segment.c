#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "segment.h"
#include "utils/matrix.h"
#include "utils/array.h"

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
    if(segment->vram) {
        free(segment->vram);
        segment->vram = NULL;
    }
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
    memcpy(buffer, segment->vram, sizeof(uint8_t) * LCD_SEGMENT_SIZE);
    Matrix vramMatrix = {
        .data = buffer,
        .height = LCD_SEGMENT_ROWS,
        .width = LCD_SEGMENT_COLUMNS
    };

    uint8_t columnData[LCD_SEGMENT_COLUMNS];
    for(uint8_t columnIndex = 0; columnIndex < LCD_SEGMENT_COLUMNS; columnIndex++) {
        Matrix_GetColumn(&vramMatrix, columnIndex, columnData);
        for(uint8_t i = 0; i < segment->startLine; i++) {
            Array_ShiftRight(columnData, LCD_SEGMENT_ROWS, columnData);
        }
        Matrix_SetColumn(&vramMatrix, columnIndex, columnData);
    }
}