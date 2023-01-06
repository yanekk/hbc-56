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

CompactFlash* CompactFlash_Create(uint8_t * data);
uint8_t CompactFlash_Status(CompactFlash *device);

uint8_t CompactFlash_SectorCount_Write(CompactFlash *device, uint8_t sectorCount);
uint8_t CompactFlash_SectorNumber_Write(CompactFlash *device, uint8_t number, uint8_t offset);
uint8_t CompactFlash_Command_ReadSector(CompactFlash *device);
uint8_t CompactFlash_Read_Byte(CompactFlash *device);
void CompactFlash_Destroy(CompactFlash * device);

#ifdef __cplusplus
}
#endif


#endif