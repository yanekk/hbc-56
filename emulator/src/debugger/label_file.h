#ifndef _HBC56_LABEL_FILE_H_
#define _HBC56_LABEL_FILE_H_

#define MEMORY_SIZE 0x10000

#ifdef __cplusplus
extern "C" {
#endif

void Debugger_LoadLabels(const char* labelFileContents, char* labelMap[MEMORY_SIZE]);

#ifdef __cplusplus
}
#endif

#endif