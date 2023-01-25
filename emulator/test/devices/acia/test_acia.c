#include "acutest.h"
#include "devices/acia/acia.h"

char buffer[64] = {0};
char* bufferPtr = buffer;

void printToBuffer(uint8_t c) {
    *bufferPtr++ = c;
}

void test_transmit_dataIsTransmitted(void)
{ 
    buffer[0] = 0;
    bufferPtr = buffer;

    ACIA* acia = ACIA_New();
    ACIA_Configure(acia, printToBuffer);
    
    ACIA_Transmit(acia, 'b');
    ACIA_Transmit(acia, 'a');
    ACIA_Transmit(acia, 'd');

    TEST_CHECK_(strcmp(buffer, "bad") == 0, "expected buffer to contain \"%s\", but got \"%s\"", "bad", buffer);

    ACIA_Free(acia);
}

TEST_LIST = {
   { "test_transmit_dataIsTransmitted", test_transmit_dataIsTransmitted },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};