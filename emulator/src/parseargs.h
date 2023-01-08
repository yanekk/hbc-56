#ifndef _HBC56_PARSEARGS_H_
#define _HBC56_PARSEARGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "devices/lcd_device.h"

typedef struct {
    char* romFile;
    bool breakOnStart;
    LCDType lcdType;
} Hbc56EmulatorArgs;

bool Hbc56EmulatorArgs_Parse(Hbc56EmulatorArgs* args, int argc, char* argv[]);

#ifdef __cplusplus
}
#endif

#endif