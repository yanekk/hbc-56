#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "SDL.h"
#include "utils/list.h"
#include "label_file.h"

static int isProbablyConstant(const char* str)
{
    char tmpBuffer[256] = {0};
    SDL_strlcpy(tmpBuffer, str, sizeof(tmpBuffer) - 1);
    SDL_strupr(tmpBuffer);
    return SDL_strcmp(str, tmpBuffer) == 0;
}

static void clearLabelMap(char* labelMap[MEMORY_SIZE]) {
  for (int i = 0; i < MEMORY_SIZE; ++i)
  {
    if(labelMap[i] == NULL)
        continue;
    free(labelMap[i]);
    labelMap[i] = NULL;
  }
}

int isnewline(int c) {
  return c == '\n';
}

static void parseToken(char** charPtrPtr, char* buffer, int (*stopPredicate)(int)) {
  char* charPtr = *charPtrPtr;
  char c = *charPtr;
  
  do {
    if(c == '\r')
      continue;
    if(stopPredicate(c)) {
      *buffer++ = 0;
      *charPtrPtr = --charPtr;
      return;
    }
    *buffer++ = c;
  } while ((c = *(++charPtr)));
}

void Debugger_LoadLabels(const char* labelFileContents, char* labelMap[MEMORY_SIZE])
{
    clearLabelMap(labelMap);

    if(labelFileContents == NULL)
        return;

    char* p = (char*)labelFileContents;
    char labelToken[64]   = {0},
         addressToken[64] = {0}, 
         commentToken[64] = {0};

    do {
      switch (*p)
      {
      case '$':
        parseToken(&p, addressToken, &isspace);
        break;
      case ';':
        parseToken(&p, commentToken, &isnewline);
        break;
      case '\n':
        if (strlen(addressToken) && strlen(labelToken)) {
          
          uint32_t address = 0;
          sscanf(addressToken, "$%x", &address);

          bool isUnused = strlen(commentToken)
            && (strstr(commentToken, "; unused") != NULL);

          //TODO: constants[std::string(lineBuffer + labelStart, labelEnd - labelStart)] = addr;
          
          if (!labelMap[address] || (isProbablyConstant(labelMap[address]) && !isUnused))
          {
            char* label = calloc(sizeof(char), strlen(labelToken) + 1);
            strcpy(label, labelToken);
            labelMap[address] = label;
          }
        }
        labelToken[0] = addressToken[0] = commentToken[0] = 0;
        break;
      case '=':
        break;
      default:
        if(!isspace(*p))
          parseToken(&p, labelToken, &isspace);
        break;
      }
    } while(*(++p));
}