#include <stdlib.h>

#include "via.h"

struct VIA {
    VIATimerState timer1State;
    uint16_t timer1InitialValue, timer1Value;
    VIATimerMode timer1mode;
    bool PB7OutputEnabled, interruptEnabled;
};

VIA* VIA_New() {
    VIA* via = malloc(sizeof(VIA));
    via->timer1State = Stopped;
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

void VIA_Timer1_SetInterrupt(VIA* via, bool isEnabled) {
    via->interruptEnabled = isEnabled;
}

bool VIA_Timer1_GetInterrupt(VIA* via) {
    return via->interruptEnabled;
}

void VIA_Free(VIA* via) {
    free(via);
}