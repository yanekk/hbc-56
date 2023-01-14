/*
 * Troy's HBC-56 Emulator - AY-3-8910 device
 *
 * Copyright (c) 2021 Troy Schrapel
 *
 * This code is licensed under the MIT license
 *
 * https://github.com/visrealm/hbc-56/emulator
 *
 */

#ifndef _HBC56_RENDERER_H_
#define _HBC56_RENDERER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "SDL.h"
#include "segment.h"

struct LcdRenderer;
typedef struct LcdRenderer LcdRenderer;

typedef struct LcdRendererImageData { uint32_t data[8192]; } LcdRendererImageData;

LcdRenderer* LcdRenderer_Create(SDL_Renderer* renderer);

SDL_Texture* LcdRenderer_GetTexture(LcdRenderer* renderer);
void LcdRenderer_Render(LcdRenderer* lcdRenderer, LcdRendererImageData* displayData);

void LcdRenderer_Destroy(LcdRenderer* renderer);

#ifdef __cplusplus
}
#endif


#endif