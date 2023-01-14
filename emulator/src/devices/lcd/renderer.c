#include <stdlib.h>

#include "renderer.h"

struct LcdRenderer {
    SDL_Texture* texture;
};

#define TEXTURE_WIDTH 128
#define TEXTURE_HEIGHT 64

#define LIME_GREEN 0x00bded7e
#define GREY 0x003e4534

LcdRenderer* LcdRenderer_Create(SDL_Renderer* renderer) {
    LcdRenderer* lcdRenderer = malloc(sizeof(LcdRenderer));
    lcdRenderer->texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    return lcdRenderer;
}

SDL_Texture* LcdRenderer_GetTexture(LcdRenderer* lcdRenderer) {
    return lcdRenderer->texture;
}

void LcdRenderer_Render(LcdRenderer* lcdRenderer, LcdRendererImageData* bitArray) {
        uint32_t* pixels;
        int pitch;

        SDL_LockTexture(lcdRenderer->texture, NULL, (void**)&pixels, &pitch );
        for(int i = 0; i < TEXTURE_WIDTH * TEXTURE_HEIGHT; i++) {
            pixels[i] = bitArray->data[i] ? GREY : LIME_GREEN;
        }
        SDL_UnlockTexture(lcdRenderer->texture);
}

void LcdRenderer_Destroy(LcdRenderer* renderer) {
    if(renderer == NULL)
        return;

    if(renderer->texture != NULL)
        SDL_DestroyTexture(renderer->texture); 

    free(renderer);
}