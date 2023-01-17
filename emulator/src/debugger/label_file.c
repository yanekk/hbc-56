#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "SDL.h"

#define MEMORY_SIZE 0x10000

static int isProbablyConstant(const char* str)
{
    char tmpBuffer[256] = {0};
    SDL_strlcpy(tmpBuffer, str, sizeof(tmpBuffer) - 1);
    SDL_strupr(tmpBuffer);
    

    return SDL_strcmp(str, tmpBuffer) == 0;
}

static void clearLabelMap(char** labelMap) {
  for (int i = 0; i < MEMORY_SIZE; ++i)
  {
    if(labelMap[i] == NULL)
        continue;
    free(labelMap[i]);
    labelMap[i] = NULL;
  }
}

void Debugger_LoadLabels(const char* labelFileContents, char* labelMap[MEMORY_SIZE])
{
    clearLabelMap(labelMap);

    if(labelFileContents == NULL)
        return;

    char lineBuffer[1024];

    char *p = (char*)labelFileContents;

    for (;;)
    {
      char* end = SDL_strchr(p, '\n');
      if (end == NULL) {
        break;
      }
        

      SDL_strlcpy(lineBuffer, p, end - p);
      p = end + 1;

      size_t labelStart = (size_t )-1, labelEnd = (size_t)-1, valueStart = (size_t)-1, valueEnd = (size_t)-1;

      int i = 0;
      int len = (int)strlen(lineBuffer);


      for (i = 0; i < len; ++i)
      {
        char c = lineBuffer[i];
        if (c == 0) break;
        if (!isspace(c) && c != '=' && c != '$')
        {
          if (labelStart == -1)
          {
            labelStart = i;
          }
          else if (labelEnd != -1 && valueStart == -1)
          {
            valueStart = i;
          }
        }
        else
        {
          if (labelStart != -1 && labelEnd == -1)
          {
            labelEnd = i;
          }
          else if (valueStart != -1 && valueEnd == -1)
          {
            valueEnd = i;
          }
        }
      }
      if (valueStart == -1)
      {
        continue;
      }
      else if (valueEnd == -1)
      {
        valueEnd = i;
      }


      char valueStr[100] = { 0 };

      // TODO: use standard library
      SDL_strlcpy(valueStr, lineBuffer + valueStart, valueEnd - valueStart + 1);

      unsigned int value = 0;

      sscanf(valueStr, "%x", &value);

      uint16_t addr = (uint16_t)value;

      bool isUnused = strstr(lineBuffer, "; unused") != NULL;

      //TODO: constants[std::string(lineBuffer + labelStart, labelEnd - labelStart)] = addr;

      if (!labelMap[addr] || (isProbablyConstant(labelMap[addr]) && !isUnused))
      {
        char* label = (char*)malloc((labelEnd - labelStart) + 1);
        SDL_strlcpy(label, lineBuffer + labelStart, labelEnd - labelStart + 1);
        labelMap[addr] = label;
      }
    }
  
}