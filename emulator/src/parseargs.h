#ifndef _HBC56_PARSEARGS_H_
#define _HBC56_PARSEARGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

typedef struct {

} Hbc56EmulatorArgs;

bool Hbc56EmulatorArgs_Parse(int argc, char* argv[], const Hbc56EmulatorArgs* args);

#ifdef __cplusplus
}
#endif

#endif