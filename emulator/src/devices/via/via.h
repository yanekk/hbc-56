#ifndef _HBC56_VIA_H_
#define _HBC56_VIA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef enum {
    Stopped,
    Running
} VIATimerState;

struct VIA;
typedef struct VIA VIA;

VIA* VIA_New();

void VIA_Timer1_Set(VIA* via, uint16_t value);
uint16_t VIA_Timer1_Get(VIA* via);

VIATimerState VIA_Timer1_State(VIA* via);

void VIA_Timer1_Start(VIA* via);
void VIA_Timer1_Decrement(VIA* via);
void VIA_Timer1_Reset(VIA* via);
void VIA_Free(VIA* via);

#ifdef __cplusplus
}
#endif

#endif