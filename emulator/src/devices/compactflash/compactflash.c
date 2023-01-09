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

struct CompactFlash {
    const uint8_t * data;
    
    bool isSectorNumberInvalid;
    uint32_t sectorNumber;

    bool isSectorCountInvalid;
    uint8_t sectorCount;

    bool hasDataRequest;
    uint16_t dataIndex;
    uint16_t dataEnd;
};

CompactFlash* CF_Create(const uint8_t * data) {
    CompactFlash* cf = malloc(sizeof(CompactFlash));
    
    cf->isSectorNumberInvalid = true;
    cf->sectorNumber = 0;    
    
    cf->isSectorCountInvalid = true;
    cf->sectorCount = 0;
    
    cf->data = data;
    cf->dataIndex = 0;
    cf->dataEnd = 0;
    cf->hasDataRequest = false;    
    return cf;
}

void CF_Destroy(CompactFlash* compactFlash) {
    if(compactFlash) {
        free(compactFlash);
    }
}

void CF_Write_SectorNumber(CompactFlash *device, uint32_t number) {
    device->isSectorNumberInvalid = false;
    device->sectorNumber = number;
}

uint32_t CF_Read_SectorNumber(CompactFlash *device) {
    return device->sectorNumber;
}

bool CF_Read_Status_DataRequest(CompactFlash *device) {
    return device->hasDataRequest;
}

void CF_Write_SectorCount(CompactFlash *device, uint8_t sectorCount) {
    device->isSectorCountInvalid = false;
    device->sectorCount = sectorCount;
}

bool CF_Read_Error_InvalidSector(CompactFlash *device) {
    return device->isSectorCountInvalid || device->isSectorNumberInvalid;
}

void CF_Write_Command_ReadSectors(CompactFlash *device) {
    device->hasDataRequest = true;
    device->dataIndex = device->sectorNumber * SECTOR_SIZE;
    device->dataEnd = device->dataIndex + device->sectorCount * SECTOR_SIZE;
}

uint8_t CF_Read_Data(CompactFlash *device, bool isDebug) {
    uint8_t data = device->data[device->dataIndex];
    if(!isDebug) {
        device->dataIndex++;
    }
    if(device->dataIndex == device->dataEnd) {
        device->hasDataRequest = false;
    }
    return data;
}

bool CF_Read_Status_Busy(CompactFlash *device) {
    return false;
}

bool CF_Read_Status_Ready(CompactFlash *device) {
    return true;
}

bool CF_Read_Status_DriveWriteFault(CompactFlash *device) {
    return false;
}

bool CF_Read_Status_MemoryCardReady(CompactFlash *device) {
    return true;
}

bool CF_Read_Status_CorrectableDataError(CompactFlash *device) {
    return false;
}

bool CF_Read_Status_Error(CompactFlash *device) {
    return false;
}

bool CF_Read_Error_BadBlock(CompactFlash *device) {
    return false;
}

bool CF_Read_Error_UncorrectableError(CompactFlash *device) {
    return false;
}

bool CF_Read_Error_InvalidCommand(CompactFlash *device) {
    return false;
}

bool CF_Read_Error_GeneralError(CompactFlash *device) {
    return false;
}