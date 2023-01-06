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
    if (CompactFlash_Read_Status_Error(cfDevice->compactFlash)) {
      *val |= 0b00000001;
    }
    if (CompactFlash_Read_Status_CorrectableDataError(cfDevice->compactFlash)) {
      *val |= 0b00000100;
    }
    if (CompactFlash_Read_Status_DataRequest(cfDevice->compactFlash)) {
      *val |= 0b00001000;
    }
    if (CompactFlash_Read_Status_MemoryCardReady(cfDevice->compactFlash)) {
      *val |= 0b00010000;
    }
    if (CompactFlash_Read_Status_DriveWriteFault(cfDevice->compactFlash)) {
      *val |= 0b00100000;
    }
    if (CompactFlash_Read_Status_Ready(cfDevice->compactFlash)) {
      *val |= 0b01000000;
    }
    if (CompactFlash_Read_Status_Busy(cfDevice->compactFlash)) {
      *val |= 0b10000000;
    }
    return 1;
  }
  if (addr == cfDevice->startAddr + CF_ERR) {
    if (CompactFlash_Read_Error_BadBlock(cfDevice->compactFlash)) {
      *val |= 0b10000000;
    }
    if (CompactFlash_Read_Error_UncorrectableError(cfDevice->compactFlash)) {
      *val |= 0b01000000;
    }
    if (CompactFlash_Read_Error_InvalidSector(cfDevice->compactFlash)) {
      *val |= 0b00010000;
    }
    if (CompactFlash_Read_Error_InvalidCommand(cfDevice->compactFlash)) {
      *val |= 0b00000100;
    }
    if (CompactFlash_Read_Error_GeneralError(cfDevice->compactFlash)) {
      *val |= 0b00000001;
    }
    return 1;
  }
  return 1;
}

static uint8_t writeCompactFlashDevice(HBC56Device* device, uint16_t addr, uint8_t val)
{
    return 1;
}