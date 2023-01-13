//#define TEST_NO_MAIN

#include <stdbool.h>
#include <string.h>

#include "utils/matrix.h"
#include "devices/lcd/segment.h"

#include "SDL.h"
#include "acutest.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

#define TEXTURE_WIDTH 128
#define TEXTURE_HEIGHT 64

#define LIME_GREEN 0x00bded7e
#define GREY 0x003e4534

void test_renderTexture(void)
{
    // act
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("my window", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED); 

    SDL_Texture* texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    
    uint32_t* pixels;
    int pitch;

    uint8_t dataA[LCD_SEGMENT_SIZE] = {0};
    Matrix matrixA = {
        .data = dataA,
        .height = 8,
        .width = 64
    };

    uint8_t dataB[LCD_SEGMENT_SIZE] = {0};

    Matrix matrixB = {
        .data = dataB,
        .height = 8,
        .width = 64
    };

    bool close = false;
    while (!close) { 
        SDL_LockTexture( texture, NULL, (void**)&pixels, &pitch );
        for(size_t i = 0; i < LCD_SEGMENT_SIZE; i++) {
            dataA[i] = rand();
            dataB[i] = rand();
        }

        Matrix_MergeToBitArray(&matrixA, &matrixB, pixels);

        for(int i = 0; i < TEXTURE_WIDTH * TEXTURE_HEIGHT; i++) {
            pixels[i] = pixels[i] ? GREY : LIME_GREEN;
        }
        SDL_UnlockTexture( texture );

        SDL_Rect dst = { .x = 0, .y = 0, .w = WINDOW_WIDTH, .h = WINDOW_HEIGHT};

        SDL_RenderCopy( renderer, texture, NULL, &dst );
        SDL_RenderPresent(renderer);

		SDL_Event event; 
		while (SDL_PollEvent(&event)) { 
			switch (event.type) { 

			case SDL_QUIT: 
				close = true; 
				break; 
		    }
        }
        SDL_Delay(25);
	} 

	SDL_DestroyTexture(texture); 
	SDL_DestroyRenderer(renderer); 
	SDL_DestroyWindow(win); 
}

TEST_LIST = {
    { "test_renderTexture", test_renderTexture },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};
