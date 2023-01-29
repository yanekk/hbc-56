#ifndef _HBC56_VIA_H_
#define _HBC56_VIA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    Stopped,
    Running
} VIATimerState;

typedef enum {
    Undefined,
    OneShot,
    Continuous
} VIATimerMode;

struct VIA;
typedef struct VIA VIA;

VIA* VIA_New();

void VIA_Timer1_Set(VIA* via, uint16_t value);
uint16_t VIA_Timer1_Get(VIA* via);

VIATimerState VIA_Timer1_State(VIA* via);

void VIA_Timer1_Start(VIA* via);
void VIA_Timer1_Decrement(VIA* via);
void VIA_Timer1_Reset(VIA* via);

void VIA_Timer1_SetMode(VIA* via, VIATimerMode mode);
VIATimerMode VIA_Timer1_GetMode(VIA* via);

void VIA_Timer1_SetPB7Output(VIA* via, bool isEnabled);
bool VIA_Timer1_GetPB7Output(VIA* via);

bool VIA_Timer1_IsInterruptEnabled(VIA* via);
void VIA_Timer1_EnableInterrupt(VIA* via);
void VIA_Timer1_DisableInterrupt(VIA* via);

bool VIA_Timer1_IsInterruptSet(VIA* via);
void VIA_Timer1_SetInterrupt(VIA* via);
void VIA_Timer1_ClearInterrupt(VIA* via);

void VIA_Free(VIA* via);

#ifdef __cplusplus
}
#endif

#endif