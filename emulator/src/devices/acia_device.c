#include <stdlib.h>
#include <stdbool.h>

#include "acia_device.h"

#define ACIA_DATA    0 // Data register
#define ACIA_STATUS  1 // Status register
#define ACIA_COMMAND 2 // Command register
#define ACIA_CONTROL 3 // Control register

uint8_t readAciaDevice(HBC56Device*, uint16_t, uint8_t*, uint8_t);
uint8_t writeAciaDevice(HBC56Device*, uint16_t, uint8_t);
void destroyAciaDevice(HBC56Device*);

typedef struct {
    uint16_t startAddress;
} AciaDevice;

HBC56Device createAciaDevice(uint16_t startAddr)
{
  HBC56Device device = createDevice("ACIA");
  
  AciaDevice* aciaDevice = malloc(sizeof(AciaDevice));
  aciaDevice->startAddress = startAddr;
  device.data = (void*)aciaDevice;

  device.readFn = &readAciaDevice;
  device.writeFn = &writeAciaDevice;
  device.destroyFn = &destroyAciaDevice;
  return device;
}

static AciaDevice* getAciaDevice(HBC56Device* device) {
    return (AciaDevice*)device->data;
}

static bool isAddressInRange(AciaDevice* device, uint16_t address) {
  return address >= device->startAddress && address <= device->startAddress + ACIA_CONTROL;
}

uint8_t readAciaDevice(HBC56Device* device, uint16_t address, uint8_t* value, uint8_t dbg) {
  AciaDevice* aciaDevice = getAciaDevice(device);
  if(aciaDevice == NULL || !isAddressInRange(aciaDevice, address))
    return 0;
  return 1;
}

uint8_t writeAciaDevice(HBC56Device* device, uint16_t address, uint8_t value) {
  AciaDevice* aciaDevice = getAciaDevice(device);
  if(aciaDevice == NULL || !isAddressInRange(aciaDevice, address))
    return 0;
  return 1;
}

void destroyAciaDevice(HBC56Device* device) {
    AciaDevice* aciaDevice = getAciaDevice(device);
    if(aciaDevice != NULL)
        free(aciaDevice);
}