#ifndef _HBC56_VIA_DEVICE_H_
#define _HBC56_VIA_DEVICE_H_

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

HBC56Device createViaDevice(uint16_t baseAddr);
void destroyViaDevice(HBC56Device* viaDevice);

#ifdef __cplusplus
}
#endif


#endif