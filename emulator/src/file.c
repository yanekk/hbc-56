#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "file.h"

File* File_Read(const char* filePath) {
    File* file = calloc(1, sizeof(File));

    FILE* filePtr = fopen(filePath, "rb");
    if(!filePtr) {
        return file;
    }
    file->isOk = true;
    fseek(filePtr, 0, SEEK_END);
    file->size = ftell(filePtr);

    file->data = calloc(sizeof(uint8_t), file->size + 1);
    rewind(filePtr);
    fread(file->data, sizeof(uint8_t), file->size, filePtr);
    fclose(filePtr);
    return file;    
}

File* File_ReadWithSuffix(const char* filePath, const char* suffix) {
    char fullFilePath[1024];
    sprintf(fullFilePath, "%s%s", filePath, suffix);
    return File_Read(fullFilePath);
}

bool File_Free(File* file) {
    if(file->data)
        free(file->data);
    if(file)
        free(file);
    return true;
}

File* File_New(const size_t fileSize) {
    File* file = calloc(1, sizeof(File));
    file->size = fileSize;
    file->isOk = true;
    file->data = calloc(sizeof(uint8_t), file->size + 1);
    return file;
}

bool File_Save(const char* fileName, const File* fileInfo) {
    FILE* filePtr = fopen(fileName,"wb");
    if(filePtr == NULL)
        return false;
    fwrite(fileInfo->data, sizeof(uint8_t), fileInfo->size, filePtr);
    fclose(filePtr);
    return true;
}

bool File_Delete(const char* fileName) {
    return remove(fileName);
}