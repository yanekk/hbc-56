#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "int.h"

void Array_ShiftRight(uint8_t* input, uint8_t size, uint8_t* output) {
    uint8_t _ = 0;

    uint8_t* inputCopy = malloc(sizeof(uint8_t) * size);
    memcpy(inputCopy, input, sizeof(uint8_t) * size);

    bool carry = Int_ShiftRightWithCarry(inputCopy[size-1], false, &_);
    for(uint8_t i = 0; i < size; i++) {
        carry = Int_ShiftRightWithCarry(inputCopy[i], carry, &output[i]);
    }
    free(inputCopy);
}