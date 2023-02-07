#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "parseargs.h"
#include "config.h"

#define OPTION_ROM "--rom"
#define OPTION_BREAK_ON_START "--brk"
#define OPTION_LCD_TYPE "--lcd"
#define OPTION_CFCARD "--cfcard"
#define OPTION_BREAKPOINT_LABEL "--breakpoint-label"

#define ERROR_FILE_NOT_FOUND "File '%s' cannot be found."
#define ERROR_INVALID_ARGUMENTS "Invalid arguments"

#define ERROR_MISSING_ROM "No HBC-56 ROM file.\n\nUse --rom <romfile>"
#define ERROR_INCORRECT_FILE_SIZE "ROM file '%s' has incorrect size. Expected: %d, got %ld."

#define ERROR_UNKNOWN_LCD_TYPE "Unknown LCD type: %s"
#define ERROR_NO_LCD_TYPE_SET "No LCD type set"

#define ERROR_MISSING_CFCARD "No CompactFlash card image file specified. Use --cfcard <filename> to set it."

#define ERROR_MISSING_BREAKPOINT_LABEL "Breakpoint label not provided. Use --breakpoint-label <label> to set it."

bool Hbc56EmulatorArgs_Parse(Hbc56EmulatorArgs* args, int argc, char* argv[], char errorBuffer[]) {
    if (argv == NULL) {
        strcpy(errorBuffer, ERROR_INVALID_ARGUMENTS);
        return false;
    }

    for(uint8_t i = 0; i < argc; i++) {
        if(strcmp(OPTION_ROM, argv[i]) == 0) {
            i++;
            if (i == argc) {
                strcpy(errorBuffer, ERROR_MISSING_ROM);
                return false;
            }
            char* romFilePath = argv[i];

            FILE* filePtr = fopen(romFilePath, "r");
            if(!filePtr) {
                sprintf(errorBuffer, ERROR_FILE_NOT_FOUND, romFilePath); 
                return false;  
            }

            fseek(filePtr, 0, SEEK_END);
            if(ftell(filePtr) != HBC56_ROM_SIZE) {
                sprintf(errorBuffer, ERROR_INCORRECT_FILE_SIZE, romFilePath, HBC56_ROM_SIZE, ftell(filePtr)); 
                fclose(filePtr);
                return false;
            }
            fclose(filePtr);       
            args->romFile = romFilePath;
        }

        if(strcmp(OPTION_CFCARD, argv[i]) == 0) {
            i++;
            if (i == argc) {
                strcpy(errorBuffer, ERROR_MISSING_CFCARD);
                return false;
            }
            char* cfcardImageFilePath = argv[i];

            FILE* filePtr = fopen(cfcardImageFilePath, "r");
            if(!filePtr) {
                sprintf(errorBuffer, ERROR_FILE_NOT_FOUND, cfcardImageFilePath); 
                return false;  
            }
            fclose(filePtr);
  
            args->cfCardImageFile = cfcardImageFilePath;
        }
        if(strcmp(OPTION_BREAK_ON_START, argv[i]) == 0) {
            args->breakOnStart = true;
        }
        if(strcmp(OPTION_LCD_TYPE, argv[i]) == 0) {
            i++;
            if (i == argc) {
                sprintf(errorBuffer, ERROR_NO_LCD_TYPE_SET); 
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
                    sprintf(errorBuffer, ERROR_UNKNOWN_LCD_TYPE, argv[i]); 
                    return false;
            }
        } 
        if(strcmp(OPTION_BREAKPOINT_LABEL, argv[i]) == 0) {
            i++;
            if (i == argc) {
                sprintf(errorBuffer, ERROR_MISSING_BREAKPOINT_LABEL); 
                return false;
            }
            args->breakpointLabel = argv[i];
        }
    }

    if (!args->romFile) {
        strcpy(errorBuffer, ERROR_MISSING_ROM);
        return false;
    }

    return true;
}