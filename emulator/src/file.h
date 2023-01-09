#ifndef _HBC56_FILE_H_
#define _HBC56_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool isOk;
    uint8_t* data;
    size_t size;
} File;

File* File_Read(const char* filePath);
File* File_ReadWithSuffix(const char* filePath, const char* suffix);
bool File_Free(File* file);

#ifdef __cplusplus
}
#endif

#endif