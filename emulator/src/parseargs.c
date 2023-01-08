#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "parseargs.h"
int doBreak = 0;

#define OPTION_ROM "--rom"
#define OPTION_BREAK_ON_START "--brk"
#define OPTION_LCD_TYPE "--lcd"

bool Hbc56EmulatorArgs_Parse(Hbc56EmulatorArgs* args, int argc, char* argv[]) {
    if (argv == NULL) {
        return false;
    }
    for(uint8_t i = 0; i < argc; i++) {
        if(strcmp(OPTION_ROM, argv[i]) == 0) {
            i++;
            if (i == argc) {
                return false;
            }
            args->romFile = argv[i];
        }
        if(strcmp(OPTION_BREAK_ON_START, argv[i]) == 0) {
            args->breakOnStart = true;
        }
        if(strcmp(OPTION_LCD_TYPE, argv[i]) == 0) {
            i++;
            if (i == argc) {
                return false;
            }
            switch (atoi(argv[i])) {
                case 1602:
                    args->lcdType = LCD_1602;
                    break;
                case 2004:
                    args->lcdType = LCD_2004;
                    break;
                case 12864:
                    args->lcdType = LCD_GRAPHICS;
                    break;
                default:
                    return false;
            }
        }        
    }
    return args;
}