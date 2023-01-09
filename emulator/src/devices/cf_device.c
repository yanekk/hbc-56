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

#include "cf_device.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "config.h"
#include "compactflash/compactflash.h"

static void resetCompactFlashDevice(HBC56Device*);
static void destroyCompactFlashDevice(HBC56Device*);
static uint8_t readCompactFlashDevice(HBC56Device*, uint16_t, uint8_t*, uint8_t);
static uint8_t writeCompactFlashDevice(HBC56Device*, uint16_t, uint8_t);

typedef struct CompactFlashDevice
{
  uint32_t startAddr;
  CompactFlash *compactFlash;
  uint8_t* data;
} CompactFlashDevice;


HBC56Device createCompactFlashDevice(uint16_t startAddr, File* imageFile)
{
  HBC56Device device = createDevice("CompactFlash");

  CompactFlashDevice* cfDevice = (CompactFlashDevice*)malloc(sizeof(CompactFlashDevice));

  if (!cfDevice) {
    destroyDevice(&device);
    return device;
  }

  cfDevice->data = malloc(sizeof(uint8_t) * imageFile->size);
  memcpy(cfDevice->data, imageFile->data, sizeof(uint8_t) * imageFile->size);
  
  CompactFlash* compactFlash = CF_Create(cfDevice->data);
  
  if (!compactFlash) {
    CF_Destroy(compactFlash);
    destroyDevice(&device);
    return device;
  }

  cfDevice->compactFlash = compactFlash;
  cfDevice->startAddr = startAddr;

  device.data = cfDevice;
  device.destroyFn = &destroyCompactFlashDevice;
  device.readFn = &readCompactFlashDevice;
  device.writeFn = &writeCompactFlashDevice;
  return device;
}

inline static CompactFlashDevice* getCompactFlashDevice(HBC56Device* device)
{
  if (!device) return NULL;
  return (CompactFlashDevice*)device->data;
}

static void destroyCompactFlashDevice(HBC56Device *device)
{
  CompactFlashDevice *cfDevice = getCompactFlashDevice(device);
  if(cfDevice->data)
    free(cfDevice->data);

  CF_Destroy(cfDevice->compactFlash);
  free(cfDevice);
  device->data = NULL;
}

static uint8_t readCompactFlashDevice(HBC56Device* device, uint16_t addr, uint8_t *val, uint8_t dbg)
{
  const CompactFlashDevice* cfDevice = getCompactFlashDevice(device);
  uint16_t offset = addr - cfDevice->startAddr;

  switch(offset) {
    case CF_STAT:
    *val |= CF_Read_Status_Error(cfDevice->compactFlash) << 0;
    *val |= CF_Read_Status_CorrectableDataError(cfDevice->compactFlash) << 2;
    *val |= CF_Read_Status_DataRequest(cfDevice->compactFlash) << 3;
    *val |= CF_Read_Status_MemoryCardReady(cfDevice->compactFlash) << 4;
    *val |= CF_Read_Status_DriveWriteFault(cfDevice->compactFlash) << 5;
    *val |= CF_Read_Status_Ready(cfDevice->compactFlash) << 6;
    *val |= CF_Read_Status_Busy(cfDevice->compactFlash) << 7;
    break;

    case CF_ERR:
    *val |= CF_Read_Error_BadBlock(cfDevice->compactFlash) << 7;
    *val |= CF_Read_Error_UncorrectableError(cfDevice->compactFlash) << 6;
    *val |= CF_Read_Error_InvalidSector(cfDevice->compactFlash) << 4;
    *val |= CF_Read_Error_InvalidCommand(cfDevice->compactFlash) << 2;
    *val |= CF_Read_Error_GeneralError(cfDevice->compactFlash) << 0;
    break;

    case CF_DATA:
    *val = CF_Read_Data(cfDevice->compactFlash, dbg);
    break;
  }

  return 1;
}

static uint8_t writeCompactFlashDevice(HBC56Device* device, uint16_t addr, uint8_t val)
{
    const CompactFlashDevice* cfDevice = getCompactFlashDevice(device);
    uint8_t offset = addr - cfDevice->startAddr;
    
    switch(offset) {
      case CF_SECCO:
      CF_Write_SectorCount(cfDevice->compactFlash, val);
      break;

      case CF_LBA0:
      CF_Write_SectorNumber(cfDevice->compactFlash,
        (CF_Read_SectorNumber(cfDevice->compactFlash) & 0xFFFFFF00) | val
      );
      break;

      case CF_LBA1:
      CF_Write_SectorNumber(cfDevice->compactFlash,
        (CF_Read_SectorNumber(cfDevice->compactFlash) & 0xFFFF00FF) | (val << 8)
      );
      break;

      case CF_LBA2:
      CF_Write_SectorNumber(cfDevice->compactFlash,
        (CF_Read_SectorNumber(cfDevice->compactFlash) & 0xFF00FFFF) | (val << 16)
      );
      break;

      case CF_LBA3:
      CF_Write_SectorNumber(cfDevice->compactFlash,
        (CF_Read_SectorNumber(cfDevice->compactFlash) & 0x00FFFFFF) | ((val << 24) & 0x1F000000)
      );
      break;

      case CF_CMD:
      switch(val) {
        case CF_Command_ReadSectors:
        CF_Write_Command_ReadSectors(cfDevice->compactFlash);
        break;
      }
      break;

    }
    return 1;
}