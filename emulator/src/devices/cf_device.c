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
} CompactFlashDevice;


HBC56Device createCompactFlashDevice(
  uint16_t startAddr,
  const uint8_t *contents)
{
  HBC56Device device = createDevice("CompactFlash");
  
  CompactFlash* compactFlash = CompactFlash_Create(contents);
  
  if (!compactFlash) {
    CompactFlash_Destroy(compactFlash);
    destroyDevice(&device);
    return device;
  }

  CompactFlashDevice* cfDevice = (CompactFlashDevice*)malloc(sizeof(CompactFlashDevice));
  if (!cfDevice) {
    CompactFlash_Destroy(compactFlash);
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
  CompactFlash_Destroy(cfDevice->compactFlash);
  free(cfDevice);
  device->data = NULL;
}

static uint8_t readCompactFlashDevice(HBC56Device* device, uint16_t addr, uint8_t *val, uint8_t dbg)
{
  const CompactFlashDevice* cfDevice = getCompactFlashDevice(device);

  if (addr == cfDevice->startAddr + CF_STAT) {
    *val |= CompactFlash_Read_Status_Error(cfDevice->compactFlash) << 0;
    *val |= CompactFlash_Read_Status_CorrectableDataError(cfDevice->compactFlash) << 2;
    *val |= CompactFlash_Read_Status_DataRequest(cfDevice->compactFlash) << 3;
    *val |= CompactFlash_Read_Status_MemoryCardReady(cfDevice->compactFlash) << 4;
    *val |= CompactFlash_Read_Status_DriveWriteFault(cfDevice->compactFlash) << 5;
    *val |= CompactFlash_Read_Status_Ready(cfDevice->compactFlash) << 6;
    *val |= CompactFlash_Read_Status_Busy(cfDevice->compactFlash) << 7;
    return 1;
  }
  if (addr == cfDevice->startAddr + CF_ERR) {
    *val |= CompactFlash_Read_Error_BadBlock(cfDevice->compactFlash) << 7;
    *val |= CompactFlash_Read_Error_UncorrectableError(cfDevice->compactFlash) << 6;
    *val |= CompactFlash_Read_Error_InvalidSector(cfDevice->compactFlash) << 4;
    *val |= CompactFlash_Read_Error_InvalidCommand(cfDevice->compactFlash) << 2;
    *val |= CompactFlash_Read_Error_GeneralError(cfDevice->compactFlash) << 0;
    return 1;
  }

  return 1;
}

static uint8_t writeCompactFlashDevice(HBC56Device* device, uint16_t addr, uint8_t val)
{
    const CompactFlashDevice* cfDevice = getCompactFlashDevice(device);

    if(addr == cfDevice->startAddr + CF_SECCO) {
      CompactFlash_Write_SectorCount(cfDevice->compactFlash, val);
    }
    if(addr == cfDevice->startAddr + CF_LBA0) {
      CompactFlash_Write_SectorNumber(cfDevice->compactFlash,
        (CompactFlash_Read_SectorNumber(cfDevice->compactFlash) & 0xFFFFFF00) | val
      );
    }
    if(addr == cfDevice->startAddr + CF_LBA1) {
      CompactFlash_Write_SectorNumber(cfDevice->compactFlash,
        (CompactFlash_Read_SectorNumber(cfDevice->compactFlash) & 0xFFFF00FF) | (val << 8)
      );
    }
    if(addr == cfDevice->startAddr + CF_LBA2) {
      CompactFlash_Write_SectorNumber(cfDevice->compactFlash,
        (CompactFlash_Read_SectorNumber(cfDevice->compactFlash) & 0xFF00FFFF) | (val << 16)
      );
    }
    if(addr == cfDevice->startAddr + CF_LBA3) {
      CompactFlash_Write_SectorNumber(cfDevice->compactFlash,
        (CompactFlash_Read_SectorNumber(cfDevice->compactFlash) & 0x00FFFFFF) | (val << 24) & 0x1F000000
      );
    }
    return 1;
}