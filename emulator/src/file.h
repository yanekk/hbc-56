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
File* File_New(const size_t fileSize);
bool File_Save(const char* fileName, const File* fileInfo);
bool File_Delete(const char* fileName);

bool File_Free(File* file);

#ifdef __cplusplus
}
#endif

#endif