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

#ifndef _HBC56_COMPACTFLASH_H_
#define _HBC56_COMPACTFLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define SECTOR_SIZE 512

struct CompactFlash;
typedef struct CompactFlash CompactFlash;

struct CompactFlash {
    const uint8_t * _data;
    
    bool _isSectorNumberInvalid;
    uint32_t _sectorNumber;

    bool _isSectorCountInvalid;
    uint8_t _sectorCount;

    bool _hasDataRequest;
    uint16_t _dataIndex;
    uint16_t _dataEnd;
};

CompactFlash* CF_Create(const uint8_t * data);

// Bit 7 (BUSY) The busy bit is set when the CompactFlash Memory Card has access to the command buffer and registers and
// the host is locked out from accessing the command register and buffer. No other bits in this register are valid
// when this bit is set to a 1.
bool CF_Read_Status_Busy(CompactFlash *device);

// Bit 6 (RDY) RDY indicates whether the device is capable of performing CompactFlash Memory Card operations. This bit
// is cleared at power up and remains cleared until the CompactFlash Card is ready to accept a command.
bool CF_Read_Status_Ready(CompactFlash *device);

// Bit 5 (DWF) This bit, if set, indicates a write fault has occurred.
bool CF_Read_Status_DriveWriteFault(CompactFlash *device);

// Bit 4 (DSC) This bit is set when the CompactFlash Memory Card is ready.
bool CF_Read_Status_MemoryCardReady(CompactFlash *device);

// Bit 3 (DRQ) The Data Request is set when the CompactFlash Memory Card requires that information be transferred either
// to or from the host through the Data register.
bool CF_Read_Status_DataRequest(CompactFlash *device);

// Bit 2 (CORR) This bit is set when a Correctable data error has been encountered and the data has been corrected. This
// condition does not terminate a multi-sector read operation.
bool CF_Read_Status_CorrectableDataError(CompactFlash *device);

// Bit 1 (IDX) This bit is always set to 0.
// Bit 0 (ERR) This bit is set when the previous command has ended in some type of error. The bits in the Error register
// contain additional information describing the error. 
bool CF_Read_Status_Error(CompactFlash *device);

// Bit 7 (BBK) This bit is set when a Bad Block is detected.
bool CF_Read_Error_BadBlock(CompactFlash *device);

// Bit 6 (UNC) This bit is set when an Uncorrectable Error is encountered.
bool CF_Read_Error_UncorrectableError(CompactFlash *device);

// Bit 5 This bit is 0.
// Bit 4 (IDNF) The requested sector ID is in error or cannot be found.
bool CF_Read_Error_InvalidSector(CompactFlash *device);

// Bit 3 This bit is 0.
// Bit 2 (Abort) This bit is set if the command has been aborted because of a CompactFlash Memory Card status condition:
// (Not Ready, Write Fault, etc.) or when an invalid command has been issued.
bool CF_Read_Error_InvalidCommand(CompactFlash *device);

// Bit 1 This bit is 0.
// Bit 0 (AMNF) This bit is set in case of a general error. 
bool CF_Read_Error_GeneralError(CompactFlash *device);

void CF_Write_SectorCount(CompactFlash *device, uint8_t sectorCount);

uint32_t CF_Read_SectorNumber(CompactFlash *device);

void CF_Write_SectorNumber(CompactFlash *device, uint32_t number);

void CF_Write_Command_ReadSectors(CompactFlash *device);

uint8_t CF_Read_Data(CompactFlash *device);

void CF_Destroy(CompactFlash * device);

#ifdef __cplusplus
}
#endif


#endif