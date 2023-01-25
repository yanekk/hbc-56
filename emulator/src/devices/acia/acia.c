#include "acia.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"

struct ACIA {
    void (*onTransmit)(uint8_t);
};

static void onTransmit(uint8_t byte) {
    printf("%c", byte);
}

ACIA* ACIA_New() {
    ACIA* acia = malloc(sizeof(ACIA));
    acia->onTransmit = onTransmit;
    if(acia == NULL)
        return NULL;
    return acia;
}

void ACIA_Configure(ACIA* acia, void (*onTransmit)(uint8_t)) {
    acia->onTransmit = onTransmit;
}

void ACIA_Transmit(ACIA* acia, uint8_t byte) {
    acia->onTransmit(byte);
}

void ACIA_Free(ACIA* acia) {
    if(acia != NULL)
        free(acia);
}
