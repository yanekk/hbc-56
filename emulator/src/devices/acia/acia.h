#ifndef _HBC56_ACIA_H_
#define _HBC56_ACIA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

struct ACIA;
typedef struct ACIA ACIA;

ACIA* ACIA_New();
void ACIA_Transmit(ACIA* acia, uint8_t byte);
void ACIA_Configure(ACIA* acia, void (*onTransmit)(uint8_t));
void ACIA_Free(ACIA* acia);

#ifdef __cplusplus
}
#endif

#endif