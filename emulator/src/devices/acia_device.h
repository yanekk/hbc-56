#ifndef _HBC56_ACIA_DEVICE_H_
#define _HBC56_ACIA_DEVICE_H_

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

HBC56Device createAciaDevice(uint16_t baseAddr);
void destroyAciaDevice(HBC56Device* aciaDevice);

#ifdef __cplusplus
}
#endif


#endif