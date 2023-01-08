#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "file.h"

File* File_Read(char* filePath) {
    File* file = calloc(1, sizeof(File));

    FILE* filePtr = fopen(filePath, "r");
    if(!filePtr) {
        return file;
    }
    file->isOk = true;
    fseek(filePtr, 0, SEEK_END);
    file->size = ftell(filePtr);

    file->data = malloc(sizeof(uint8_t) * file->size + 1);
    rewind(filePtr);
    fread(file->data, sizeof(uint8_t), file->size, filePtr);
    fclose(filePtr);
    return file;    
}

bool File_Free(File* file) {
    free(file->data);
    free(file);
    return true;
}