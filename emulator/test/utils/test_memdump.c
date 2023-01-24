#include "acutest.h"
#include "utils/memdump.h"
#include "file.h"

#define FIRST_BYTE 0xFB
#define LAST_BYTE 0xBF
#define MEMORY_SIZE 0x10000

uint8_t readMemory(uint16_t address, bool isDebug) {
    if(address == 0)
        return FIRST_BYTE;
    if(address == MEMORY_SIZE-1)
        return LAST_BYTE;
    return 0;
}

void test_save_memoryIsDumped() {
    // act
    MemDump_Save(readMemory);

    // assert
    File* memDump = File_Read("memdump.bin");

    TEST_CHECK_(memDump->isOk, "Memory dump file should be created");
    TEST_CHECK_(memDump->size == MEMORY_SIZE, "Memory dump should have 65 536 of memory size, got %d", memDump->size);
    TEST_CHECK_(memDump->data[0] == FIRST_BYTE, "First byte should be equal to %x, got %x",FIRST_BYTE, memDump->data[0]);
    TEST_CHECK_(memDump->data[MEMORY_SIZE-1] == LAST_BYTE, "Last byte should be equal to %x, got %x", LAST_BYTE, memDump->data[0]);

    // cleanup
    File_Delete("memdump.bin");
    File_Free(memDump);
}

TEST_LIST = {
    { "test_save_memoryIsDumped", test_save_memoryIsDumped },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};
