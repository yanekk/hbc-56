#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "parseargs.h"
int doBreak = 0;


bool Hbc56EmulatorArgs_Parse(Hbc56EmulatorArgs* args, int argc, char* argv[]) {
    if (argv == NULL) {
        return NULL;
    }
    for(uint8_t i = 0; i < argc; i++) {
        if(strcmp("--rom", argv[i]) == 0) {
            args->romFile = argv[++i];
        }
    }
    return args;

//   /* parse arguments */
//   for (int i = 1; i < argc;)
//   {
//     int consumed;

//     consumed = 0;//SDLCommonArg(state, i);
//     if (consumed <= 0)
//     {
//       consumed = -1;
//       if (SDL_strcasecmp(argv[i], "--rom") == 0)
//       {
//         if (argv[i + 1])
//         {
//           consumed = 1;
//           romLoaded = loadRom(argv[++i]);
//         }
//       }
//       /* start paused? */
//       else if (SDL_strcasecmp(argv[i], "--brk") == 0)
//       {
//         consumed = 1;
//         doBreak = 1;
//       }
//       /* enable the lcd? */
//       else if (SDL_strcasecmp(argv[i], "--lcd") == 0)
//       {
//         if (argv[i + 1])
//         {
//           consumed = 1;
//           switch (atoi(argv[i + 1]))
//           {
//           case 1602:
//             lcdType = LCD_1602;
//             break;
//           case 2004:
//             lcdType = LCD_2004;
//             break;
//           case 12864:
//             lcdType = LCD_GRAPHICS;
//             break;
//           }
//           ++i;
//         }
//       }
//     }
//     if (consumed < 0)
//     {
//       static const char* options[] = { "--rom <romfile>","[--brk]","[--keyboard]", NULL };
//       //SDLCommonLogUsage(state, argv[0], options);
//       return 2;
//     }
//     i += consumed;
//   }
}