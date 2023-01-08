#ifndef _HBC56_PARSEARGS_H_
#define _HBC56_PARSEARGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

typedef struct {
    char* romFile;
} Hbc56EmulatorArgs;

Hbc56EmulatorArgs* Hbc56EmulatorArgs_Parse(int argc, char* argv[]);

#ifdef __cplusplus
}
#endif

#endif