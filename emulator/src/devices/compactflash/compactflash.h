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

struct CompactFlash;
typedef struct CompactFlash CompactFlash;

struct CompactFlash {
    uint8_t * _data;
};

CompactFlash* CompactFlash_Create(const uint8_t * data);
// todo: split into separate status bit functions

// // Bit 7 (BUSY) The busy bit is set when the CompactFlash Memory Card has access to the command buffer and registers and
// // the host is locked out from accessing the command register and buffer. No other bits in this register are valid
// // when this bit is set to a 1.
bool CompactFlash_Read_Status_Busy(CompactFlash *device);

// // Bit 6 (RDY) RDY indicates whether the device is capable of performing CompactFlash Memory Card operations. This bit
// // is cleared at power up and remains cleared until the CompactFlash Card is ready to accept a command.
bool CompactFlash_Read_Status_Ready(CompactFlash *device);

// // Bit 5 (DWF) This bit, if set, indicates a write fault has occurred.
bool CompactFlash_Read_Status_DriveWriteFault(CompactFlash *device);

// // Bit 4 (DSC) This bit is set when the CompactFlash Memory Card is ready.
bool CompactFlash_Read_Status_MemoryCardReady(CompactFlash *device);

// // Bit 3 (DRQ) The Data Request is set when the CompactFlash Memory Card requires that information be transferred either
// // to or from the host through the Data register.
bool CompactFlash_Read_Status_DataRequest(CompactFlash *device);

// // Bit 2 (CORR) This bit is set when a Correctable data error has been encountered and the data has been corrected. This
// // condition does not terminate a multi-sector read operation.
bool CompactFlash_Read_Status_CorrectableDataError(CompactFlash *device);

// // Bit 1 (IDX) This bit is always set to 0.
// // Bit 0 (ERR) This bit is set when the previous command has ended in some type of error. The bits in the Error register
// // contain additional information describing the error. 
bool CompactFlash_Read_Status_Error(CompactFlash *device);

// // Bit 7 (BBK) This bit is set when a Bad Block is detected.
// uint8_t CompactFlash_Read_Error_BadBlock(CompactFlash *device);

// // Bit 6 (UNC) This bit is set when an Uncorrectable Error is encountered.
// uint8_t CompactFlash_Read_Error_UncorrectableError(CompactFlash *device);

// // Bit 5 This bit is 0.
// // Bit 4 (IDNF) The requested sector ID is in error or cannot be found.
// uint8_t CompactFlash_Read_Error_InvalidSector(CompactFlash *device);

// // Bit 3 This bit is 0.
// // Bit 2 (Abort) This bit is set if the command has been aborted because of a CompactFlash Memory Card status condition:
// // (Not Ready, Write Fault, etc.) or when an invalid command has been issued.
// uint8_t CompactFlash_Read_Error_InvalidCommand(CompactFlash *device);

// // Bit 1 This bit is 0.
// // Bit 0 (AMNF) This bit is set in case of a general error. 
// uint8_t CompactFlash_Read_Error_GeneralError(CompactFlash *device);

void CompactFlash_Write_SectorCount(CompactFlash *device, uint8_t sectorCount);

// uint32_t CompactFlash_Read_SectorNumber(CompactFlash *device);
// void CompactFlash_Write_SectorNumber(CompactFlash *device, uint32_t number);

// uint8_t CompactFlash_Write_Command_ReadSector(CompactFlash *device);
// uint8_t CompactFlash_Read_Data(CompactFlash *device);

void CompactFlash_Destroy(CompactFlash * device);

#ifdef __cplusplus
}
#endif


#endif