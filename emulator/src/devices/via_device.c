#include <stdlib.h>

#include "via_device.h"
#include "devices/via/via.h"
#include "hbc56emu.h"

uint8_t readViaDevice(HBC56Device*, uint16_t, uint8_t*, uint8_t);
uint8_t writeViaDevice(HBC56Device*, uint16_t, uint8_t);
void tickViaDevice(HBC56Device*,uint32_t,double);
uint8_t readViaDevice(HBC56Device*, uint16_t, uint8_t*, uint8_t);
void destroyViaDevice(HBC56Device*);

typedef struct {
    uint16_t startAddress;
    VIA* via;
} ViaDevice;

HBC56Device createViaDevice(uint16_t startAddress)
{
  HBC56Device device = createDevice("VIA");

  ViaDevice* viaDevice = malloc(sizeof(ViaDevice));
  viaDevice->startAddress = startAddress;
  viaDevice->via = VIA_New();

  device.data = (void*)viaDevice;
  device.readFn = &readViaDevice;
  device.writeFn = &writeViaDevice;
  device.destroyFn = &destroyViaDevice;
  device.tickFn = &tickViaDevice;
  return device;
}

static ViaDevice* getViaDevice(HBC56Device* device) {
    return (ViaDevice*)device->data;
}

static bool isAddressInRange(ViaDevice* device, uint16_t address) {
  return address >= device->startAddress 
      && address <= device->startAddress + VIA_IER;
}

uint8_t readViaDevice(HBC56Device* device, uint16_t address, uint8_t* value, uint8_t dbg) {
  ViaDevice* viaDevice = getViaDevice(device);
  if(viaDevice == NULL || !isAddressInRange(viaDevice, address))
    return 0;
  switch(address - viaDevice->startAddress) {
    case VIA_IFR:
    *value = 0;
    if(VIA_Timer1_IsInterruptSet(viaDevice->via)) {
      *value |= VIA_IFR_TIMER1;
    }
    if(*value != 0) {
      *value |= VIA_IFR_ANY;
    }
    break;
  }
  return 1;
}

uint8_t writeViaDevice(HBC56Device* device, uint16_t address, uint8_t value) {
  ViaDevice* viaDevice = getViaDevice(device);
  if(viaDevice == NULL || !isAddressInRange(viaDevice, address))
    return 0;

  switch(address - viaDevice->startAddress) {
    case VIA_ACR:
    VIA_Timer1_SetMode(viaDevice->via, (value & VIA_ACR_TIMER1_CONTINOUS_MODE) != 0 ? Continuous : OneShot);
    VIA_Timer1_SetPB7Output(viaDevice->via, (value & VIA_ACR_TIMER1_ENABLE_PB7_OUTPUT) != 0);
    break;

    case VIA_IER:
    if((value & VIA_IER_SET) != 0) {
      if((value & VIA_IER_TIMER1) != 0)
        VIA_Timer1_EnableInterrupt(viaDevice->via);
    } else {
      if((value & VIA_IER_TIMER1) != 0)
        VIA_Timer1_DisableInterrupt(viaDevice->via);
    }
    break;

    case VIA_T1CL:
    VIA_Timer1_Set(viaDevice->via, ((VIA_Timer1_Get(viaDevice->via) & 0xFF00) | value));
    break;

    case VIA_T1CH:
    if(VIA_Timer1_IsInterruptSet(viaDevice->via)) {
      VIA_Timer1_ClearInterrupt(viaDevice->via);
      hbc56Interrupt(1, INTERRUPT_RELEASE);
    } else {
      VIA_Timer1_Set(viaDevice->via, ((VIA_Timer1_Get(viaDevice->via) & 0x00FF) | (value << 8)));
      VIA_Timer1_Start(viaDevice->via);
    }
    break;
  }
  return 1;
}

void tickViaDevice(HBC56Device* device, uint32_t deltaTicks, double deltaTime)
{
  ViaDevice* viaDevice = getViaDevice(device);
  while(deltaTicks-- && (VIA_Timer1_State(viaDevice->via) == Running) && !VIA_Timer1_IsInterruptSet(viaDevice->via)) {
    VIA_Timer1_Decrement(viaDevice->via);
    if(VIA_Timer1_State(viaDevice->via) == Stopped) { // timer is at 0
      if(VIA_Timer1_IsInterruptEnabled(viaDevice->via)) {
        VIA_Timer1_SetInterrupt(viaDevice->via);
        hbc56Interrupt(1, INTERRUPT_RAISE);
      }
      if(VIA_Timer1_GetMode(viaDevice->via) == Continuous) {
        VIA_Timer1_Reset(viaDevice->via);
        VIA_Timer1_Start(viaDevice->via);
      }
    }
  }    
}

void destroyViaDevice(HBC56Device* device)
{
    ViaDevice* viaDevice = getViaDevice(device);
    
    if(viaDevice != NULL) {
      if(viaDevice->via != NULL)
        VIA_Free(viaDevice->via);
      free(viaDevice);
      device->data = NULL;
    }    
}