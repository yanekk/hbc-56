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

#include "compactflash.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

CompactFlash* CF_Create(const uint8_t * data) {
    CompactFlash* cf = malloc(sizeof(CompactFlash));
    cf->_data = data;
    return cf;
}

void CF_Destroy(CompactFlash* compactFlash) {
    if(compactFlash) {
        free(compactFlash);
    }
}

void CF_Write_SectorNumber(CompactFlash *device, uint32_t number) {
    device->_sectorNumber = number;
}


uint32_t CF_Read_SectorNumber(CompactFlash *device) {
    return device->_sectorNumber;
}

