#ifndef _HBC56_PARSEARGS_H_
#define _HBC56_PARSEARGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "devices/lcd_device.h"

typedef struct {
    char* romFile;
    char* cfCardImageFile;
    bool breakOnStart;
    LCDType lcdType;
    char* exitLabel;
    char* dumpMemoryFile;
} Hbc56EmulatorArgs;

bool Hbc56EmulatorArgs_Parse(Hbc56EmulatorArgs* args, int argc, char* argv[], char errorBuffer[]);

#ifdef __cplusplus
}
#endif

#endif