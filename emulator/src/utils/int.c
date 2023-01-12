#include "int.h"

bool Int_ShiftRightWithCarry(uint8_t input, bool carry, uint8_t* output) {
    uint16_t longInput = (uint16_t)input << 7;
    *output = (uint8_t)(longInput >> 8 | carry << 7);
    return (bool)((uint8_t)longInput >> 7);
}