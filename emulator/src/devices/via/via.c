#include <stdlib.h>

#include "via.h"

struct VIA {
    VIATimerState timer1State;
    uint16_t timer1InitialValue, timer1Value;
    VIATimerMode timer1mode;
    bool PB7OutputEnabled, interruptEnabled, interruptSet;
};

VIA* VIA_New() {
    VIA* via = malloc(sizeof(VIA));
    via->timer1State      = Stopped;
    via->interruptSet     = false;
    via->interruptEnabled = false;
    via->PB7OutputEnabled = false;
    return via;
}

void VIA_Timer1_Set(VIA* via, uint16_t value) {
    via->timer1InitialValue = value;
    if(via->timer1State == Stopped)
        via->timer1Value = value;
}

uint16_t VIA_Timer1_Get(VIA* via) {
    return via->timer1Value;
}

VIATimerState VIA_Timer1_State(VIA* via) {
    return via->timer1State;
}

void VIA_Timer1_Start(VIA* via) {
    via->timer1State = Running;
}

void VIA_Timer1_Decrement(VIA* via) {
    if(via->timer1State == Running)
        via->timer1Value--;
        
    if(via->timer1Value == 0)
        via->timer1State = Stopped;
}

void VIA_Timer1_Reset(VIA* via) {
    via->timer1Value = via->timer1InitialValue;
    via->timer1State = Stopped;
}

void VIA_Timer1_SetMode(VIA* via, VIATimerMode mode) {
    via->timer1mode = mode;
}

VIATimerMode VIA_Timer1_GetMode(VIA* via) {
    return via->timer1mode;
}

void VIA_Timer1_SetPB7Output(VIA* via, bool isEnabled) {
    via->PB7OutputEnabled = isEnabled;
}

bool VIA_Timer1_GetPB7Output(VIA* via) {
    return via->PB7OutputEnabled;
}

bool VIA_Timer1_IsInterruptEnabled(VIA* via) {
    return via->interruptEnabled;
}

void VIA_Timer1_EnableInterrupt(VIA* via) {
    via->interruptEnabled = true;
}

void VIA_Timer1_DisableInterrupt(VIA* via) {
    via->interruptEnabled = false;
}

bool VIA_Timer1_IsInterruptSet(VIA* via) {
    return via->interruptSet;
}

void VIA_Timer1_SetInterrupt(VIA* via) {
    via->interruptSet = true;
}

void VIA_Timer1_ClearInterrupt(VIA* via) {
    via->interruptSet = false;
}

void VIA_Free(VIA* via) {
    free(via);
}