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
    
    cf->_isSectorNumberInvalid = true;
    cf->_sectorNumber = 0;    
    
    cf->_isSectorCountInvalid = true;
    cf->_sectorCount = 0;
    
    cf->_data = data;
    cf->_dataIndex = 0;
    cf->_dataEnd = 0;
    cf->_hasDataRequest = false;    
    return cf;
}

void CF_Destroy(CompactFlash* compactFlash) {
    if(compactFlash) {
        free(compactFlash);
    }
}

void CF_Write_SectorNumber(CompactFlash *device, uint32_t number) {
    device->_isSectorNumberInvalid = false;
    device->_sectorNumber = number;
}

uint32_t CF_Read_SectorNumber(CompactFlash *device) {
    return device->_sectorNumber;
}

bool CF_Read_Status_DataRequest(CompactFlash *device) {
    return device->_hasDataRequest;
}

void CF_Write_SectorCount(CompactFlash *device, uint8_t sectorCount) {
    device->_isSectorCountInvalid = false;
    device->_sectorCount = sectorCount;
}

bool CF_Read_Error_InvalidSector(CompactFlash *device) {
    return device->_isSectorCountInvalid || device->_isSectorNumberInvalid;
}

void CF_Write_Command_ReadSectors(CompactFlash *device) {
    device->_hasDataRequest = true;
    device->_dataIndex = device->_sectorNumber * SECTOR_SIZE;
    device->_dataEnd = device->_dataIndex + device->_sectorCount * SECTOR_SIZE;
}

uint8_t CF_Read_Data(CompactFlash *device) {
    uint8_t data = device->_data[device->_dataIndex++];
    if(device->_dataIndex == device->_dataEnd) {
        device->_hasDataRequest = false;
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