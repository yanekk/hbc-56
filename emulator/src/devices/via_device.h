#ifndef _HBC56_VIA_DEVICE_H_
#define _HBC56_VIA_DEVICE_H_

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VIA_PB   0
#define VIA_PA   1
#define VIA_DDRB 2
#define VIA_DDRA 3
#define VIA_T1CL 4
#define VIA_T1CH 5
#define VIA_T1LL 6
#define VIA_T1LH 7
#define VIA_T2CL 8
#define VIA_T2CH 9
#define VIA_SR   10
#define VIA_ACR  11
#define VIA_PCR  12
#define VIA_IFR  13
#define VIA_IER  14

HBC56Device createViaDevice(uint16_t baseAddr);
void destroyViaDevice(HBC56Device* viaDevice);

#ifdef __cplusplus
}
#endif


#endif