#include <stdlib.h>
#include <stdbool.h>

#include "segment.h"

struct LcdSegment {

};

LcdSegment* LcdSegment_Create() {
    return malloc(sizeof(LcdSegment));
}

bool LcdSegment_Destroy(LcdSegment* segment) {
    if(segment != NULL) {
        free(segment);
    }
    return true;
}