/*
 * Troy's HBC-56 Emulator - Audio
 *
 * Copyright (c) 2021 Troy Schrapel
 *
 * This code is licensed under the MIT license
 *
 * https://github.com/visrealm/hbc-56/emulator
 *
 */

#ifndef _HBC56_LIST_H_
#define _HBC56_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

struct List;
typedef struct List List;

List* List_Create();
uint16_t List_Size(List* list);
void List_Append(List* list, void* item);
void List_Destroy(List* list);
void* List_At(List* list, uint16_t index);

#ifdef __cplusplus
}
#endif

#endif