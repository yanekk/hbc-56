#ifndef _HBC56_PARSEARGS_H_
#define _HBC56_PARSEARGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

typedef struct {
    char* romFile;
    bool breakOnStart;
} Hbc56EmulatorArgs;

bool Hbc56EmulatorArgs_Parse(Hbc56EmulatorArgs* args, int argc, char* argv[]);

#ifdef __cplusplus
}
#endif

#endif