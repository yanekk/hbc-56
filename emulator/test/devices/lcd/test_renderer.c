//#define TEST_NO_MAIN

#include <stdbool.h>
#include <string.h>

#include "utils/matrix.h"
#include "devices/lcd/segment.h"
#include "devices/lcd/renderer.h"

#include "SDL.h"
#include "acutest.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define TEXTURE_WIDTH 128
#define TEXTURE_HEIGHT 64

void test_create() {
    // arrange
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("my window", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED); 

    // act
    LcdRenderer* lcdRenderer = LcdRenderer_Create(renderer);
    
    // assert
    SDL_Texture* texture = LcdRenderer_GetTexture(lcdRenderer);
    TEST_ASSERT(texture != NULL);

    // cleanup
	SDL_DestroyRenderer(renderer); 
	SDL_DestroyWindow(win); 
}



void test_renderTexture(void)
{
    // act
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("my window", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED); 

    // act
    LcdRenderer* lcdRenderer = LcdRenderer_Create(renderer);
    
    LcdRendererImageData imageData;
    for(size_t i = 0; i < TEXTURE_WIDTH*TEXTURE_HEIGHT; i++) {
        imageData.data[i] = ((uint8_t)rand()) & 1;
    }

    LcdRenderer_Render(lcdRenderer, &imageData);

    SDL_Rect dst = { .x = 0, .y = 0, .w = WINDOW_WIDTH, .h = WINDOW_HEIGHT};

    SDL_RenderCopy( renderer, LcdRenderer_GetTexture(lcdRenderer), NULL, &dst );
    SDL_RenderPresent(renderer);

    LcdRenderer_Destroy(lcdRenderer);
	SDL_DestroyRenderer(renderer); 
	SDL_DestroyWindow(win); 
}

TEST_LIST = {
    { "test_create", test_create },
    { "test_renderTexture", test_renderTexture },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};
