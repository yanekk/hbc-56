#include "memdump.h"
#include "file.h"
#include <stddef.h>
#define MEMORY_SIZE 0x10000
#define MEMORY_DUMP_FILE "memdump.bin"

void MemDump_SaveToFile(uint8_t (readMemory)(uint16_t, bool), const char* filePath) {
    File* file = File_New(MEMORY_SIZE);

    for(size_t address = 0; address < MEMORY_SIZE; address++) {
        file->data[address] = readMemory(address, true);
    }
    File_Save(filePath, file);
    File_Free(file);
}

void MemDump_Save(uint8_t (readMemory)(uint16_t, bool)) {
    MemDump_SaveToFile(readMemory, MEMORY_DUMP_FILE);
}