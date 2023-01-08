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

#include "cf_device.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "config.h"
#include "compactflash/compactflash.h"

// array size is 3072
static const uint8_t cf_card[]  = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x5a, 0xa0, 0x04, 0x20, 0x00, 0xff, 0x7a, 0x5a, 0xa0, 0x00, 0x20, 0x00, 0xff, 0x7a, 0x5a, 0xa0, 
  0x01, 0x20, 0x00, 0xff, 0x7a, 0xa9, 0xf2, 0xa2, 0x3e, 0x5a, 0xa0, 0x02, 0x20, 0x00, 0xff, 0x7a, 
  0xa9, 0x02, 0x85, 0x03, 0xa9, 0x04, 0x85, 0x00, 0xa9, 0x00, 0x85, 0x01, 0xa9, 0x00, 0x85, 0x02, 
  0xa2, 0x00, 0xa0, 0x02, 0x20, 0xc2, 0x3e, 0x5a, 0xa0, 0x01, 0x20, 0x00, 0xff, 0x7a, 0x4c, 0x00, 
  0x02, 0x20, 0x6c, 0x3e, 0x20, 0x76, 0x3e, 0xa9, 0x04, 0x8d, 0x07, 0x50, 0xad, 0x01, 0x50, 0x20, 
  0x6c, 0x3e, 0xa9, 0x01, 0x8d, 0x01, 0x50, 0x20, 0x6c, 0x3e, 0xad, 0x01, 0x50, 0x20, 0x76, 0x3e, 
  0xa9, 0xef, 0x8d, 0x07, 0x50, 0x20, 0x6c, 0x3e, 0xad, 0x01, 0x50, 0x60, 0xad, 0x07, 0x50, 0x29, 
  0x80, 0xc9, 0x00, 0xd0, 0xf7, 0x60, 0xad, 0x07, 0x50, 0x29, 0xc0, 0x49, 0x40, 0xc9, 0x00, 0xd0, 
  0xf5, 0x60, 0xad, 0x07, 0x50, 0x29, 0x88, 0x49, 0x08, 0xc9, 0x00, 0xd0, 0xf5, 0x60, 0xe6, 0x04, 
  0xd0, 0x02, 0xe6, 0x05, 0x60, 0x48, 0x20, 0x76, 0x3e, 0x68, 0xa9, 0x20, 0x8d, 0x07, 0x50, 0x20, 
  0x82, 0x3e, 0xad, 0x07, 0x50, 0x29, 0x01, 0xc9, 0x00, 0xd0, 0xea, 0x20, 0x82, 0x3e, 0xad, 0x00, 
  0x50, 0xa0, 0x00, 0x91, 0x04, 0x20, 0x8e, 0x3e, 0xad, 0x07, 0x50, 0x29, 0x08, 0xc9, 0x08, 0xf0, 
  0xed, 0x60, 0x86, 0x04, 0x84, 0x05, 0xa5, 0x03, 0x8d, 0x02, 0x50, 0x20, 0x6c, 0x3e, 0xa5, 0x00, 
  0x8d, 0x03, 0x50, 0x20, 0x6c, 0x3e, 0xa5, 0x01, 0x8d, 0x04, 0x50, 0x20, 0x6c, 0x3e, 0xa5, 0x02, 
  0x8d, 0x05, 0x50, 0x20, 0x6c, 0x3e, 0xa9, 0xe0, 0x8d, 0x06, 0x50, 0x20, 0x6c, 0x3e, 0x20, 0x95, 
  0x3e, 0x60, 0x4c, 0x6f, 0x61, 0x64, 0x69, 0x6e, 0x67, 0x2e, 0x2e, 0x2e, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xa9, 0xc1, 0xa2, 0x05, 0x5a, 0xa0, 0x02, 0x20, 0x00, 0xff, 0x7a, 0x20, 0x46, 0x02, 0xa9, 0x00, 
  0x20, 0x72, 0x02, 0x20, 0xb6, 0x02, 0xa9, 0xff, 0x8d, 0x04, 0x60, 0x8d, 0x05, 0x60, 0xa9, 0x40, 
  0x8d, 0x0b, 0x60, 0xa9, 0xc0, 0x8d, 0x0e, 0x60, 0x58, 0x4c, 0x3b, 0x02, 0x20, 0xc9, 0x02, 0xc6, 
  0x11, 0xa5, 0x11, 0xc9, 0x00, 0xd0, 0x04, 0xa9, 0x40, 0x85, 0x11, 0xa5, 0xfd, 0xf0, 0xfc, 0xa9, 
  0x00, 0x85, 0xfd, 0x4c, 0x2c, 0x02, 0xa9, 0x00, 0x85, 0x04, 0xa9, 0x42, 0x85, 0x05, 0xa9, 0x01, 
  0x85, 0x06, 0xa9, 0x42, 0x85, 0x07, 0x20, 0x6d, 0x02, 0xa9, 0x00, 0x85, 0x04, 0xa9, 0x44, 0x85, 
  0x05, 0xa9, 0x01, 0x85, 0x06, 0xa9, 0x44, 0x85, 0x07, 0x20, 0x6d, 0x02, 0x60, 0xa9, 0x3f, 0x92, 
  0x04, 0x60, 0x85, 0x10, 0xa9, 0x00, 0x85, 0x04, 0xa9, 0x42, 0x85, 0x05, 0xa9, 0x01, 0x85, 0x06, 
  0xa9, 0x42, 0x85, 0x07, 0x20, 0x9b, 0x02, 0xa9, 0x00, 0x85, 0x04, 0xa9, 0x44, 0x85, 0x05, 0xa9, 
  0x01, 0x85, 0x06, 0xa9, 0x44, 0x85, 0x07, 0x20, 0x9b, 0x02, 0x60, 0xa0, 0x00, 0xc0, 0x08, 0xf0, 
  0x14, 0x98, 0x09, 0xb8, 0x92, 0x04, 0xc8, 0xa2, 0x00, 0xe0, 0x40, 0xf0, 0xf0, 0xa5, 0x10, 0x92, 
  0x06, 0xe8, 0x4c, 0xa9, 0x02, 0x60, 0xa2, 0x00, 0xbd, 0xad, 0x05, 0x9d, 0x00, 0x3d, 0x0a, 0x0a, 
  0x0a, 0x0a, 0x9d, 0x00, 0x3f, 0xe8, 0xd0, 0xf0, 0x60, 0xa9, 0x9d, 0x85, 0x0a, 0xa9, 0x03, 0x85, 
  0x0b, 0xa9, 0x00, 0x85, 0x09, 0xa2, 0x00, 0xe0, 0x00, 0xd0, 0x22, 0xa9, 0x00, 0x85, 0x04, 0xa9, 
  0x42, 0x85, 0x05, 0xa9, 0x01, 0x85, 0x06, 0xa9, 0x42, 0x85, 0x07, 0xa5, 0x11, 0x09, 0xc0, 0x92, 
  0x04, 0xa9, 0xb8, 0x05, 0x09, 0x92, 0x04, 0xa9, 0x40, 0x09, 0x00, 0x92, 0x04, 0xe0, 0x10, 0xd0, 
  0x22, 0xa9, 0x00, 0x85, 0x04, 0xa9, 0x44, 0x85, 0x05, 0xa9, 0x01, 0x85, 0x06, 0xa9, 0x44, 0x85, 
  0x07, 0xa5, 0x11, 0x09, 0xc0, 0x92, 0x04, 0xa9, 0xb8, 0x05, 0x09, 0x92, 0x04, 0xa9, 0x40, 0x09, 
  0x00, 0x92, 0x04, 0x8a, 0xa8, 0xb1, 0x0a, 0x85, 0x0c, 0x18, 0xa9, 0x20, 0x7d, 0x7b, 0x03, 0xa8, 
  0xb1, 0x0a, 0xa8, 0xda, 0xa6, 0x0c, 0xbd, 0x00, 0x3d, 0x19, 0x00, 0x3f, 0x92, 0x06, 0xe8, 0xc8, 
  0xbd, 0x00, 0x3d, 0x19, 0x00, 0x3f, 0x92, 0x06, 0xe8, 0xc8, 0xbd, 0x00, 0x3d, 0x19, 0x00, 0x3f, 
  0x92, 0x06, 0xe8, 0xc8, 0xbd, 0x00, 0x3d, 0x19, 0x00, 0x3f, 0x92, 0x06, 0xfa, 0xe8, 0xe0, 0x20, 
  0xd0, 0x15, 0xa4, 0x09, 0xc8, 0x84, 0x09, 0xc0, 0x08, 0xf0, 0x0f, 0xb9, 0x9d, 0x05, 0x85, 0x0a, 
  0xb9, 0xa5, 0x05, 0x85, 0x0b, 0xa2, 0x00, 0x4c, 0xd7, 0x02, 0x60, 0x00, 0x01, 0x02, 0x03, 0x04, 
  0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 
  0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x08, 0x0c, 0x10, 
  0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 
  0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x00, 0x04, 0x10, 
  0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 
  0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x08, 0x0c, 
  0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 
  0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x10, 0x00, 0x04, 
  0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 
  0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x08, 0x0c, 0x10, 
  0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 
  0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x00, 0x04, 0x10, 
  0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 
  0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x08, 0x0c, 
  0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 
  0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x10, 0x00, 0x04, 
  0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 
  0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x08, 0x0c, 0x10, 
  0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 
  0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x00, 0x04, 0x10, 
  0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 
  0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x08, 0x0c, 
  0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 
  0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x10, 0x00, 0x04, 
  0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 
  0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x08, 0x0c, 0x10, 
  0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 
  0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x00, 0x04, 0x10, 
  0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 
  0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x08, 0x0c, 
  0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 
  0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x0c, 0x10, 0x08, 0x10, 0x00, 0x04, 
  0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 
  0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x04, 0x10, 0x00, 0x9d, 0xdd, 0x1d, 
  0x5d, 0x9d, 0xdd, 0x1d, 0x5d, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x03, 0x07, 0x0f, 
  0x0f, 0x0f, 0x0f, 0x07, 0x03, 0x0c, 0x0e, 0x0f, 0x0f, 0x0f, 0x0f, 0x0e, 0x0c, 0x00, 0x00, 0x00, 
  0x00, 0x50, 0x72, 0x6f, 0x67, 0x72, 0x61, 0x6d, 0x20, 0x6c, 0x6f, 0x61, 0x64, 0x65, 0x64, 0x00
};

static void resetCompactFlashDevice(HBC56Device*);
static void destroyCompactFlashDevice(HBC56Device*);
static uint8_t readCompactFlashDevice(HBC56Device*, uint16_t, uint8_t*, uint8_t);
static uint8_t writeCompactFlashDevice(HBC56Device*, uint16_t, uint8_t);

typedef struct CompactFlashDevice
{
  uint32_t startAddr;
  CompactFlash *compactFlash;
} CompactFlashDevice;


HBC56Device createCompactFlashDevice(
  uint16_t startAddr,
  const uint8_t *contents)
{
  HBC56Device device = createDevice("CompactFlash");

  CompactFlash* compactFlash = CF_Create(cf_card);
  
  if (!compactFlash) {
    CF_Destroy(compactFlash);
    destroyDevice(&device);
    return device;
  }

  CompactFlashDevice* cfDevice = (CompactFlashDevice*)malloc(sizeof(CompactFlashDevice));
  if (!cfDevice) {
    CF_Destroy(compactFlash);
    destroyDevice(&device);
    return device;
  }

  cfDevice->compactFlash = compactFlash;
  cfDevice->startAddr = startAddr;

  device.data = cfDevice;
  device.destroyFn = &destroyCompactFlashDevice;
  device.readFn = &readCompactFlashDevice;
  device.writeFn = &writeCompactFlashDevice;
  return device;
}

inline static CompactFlashDevice* getCompactFlashDevice(HBC56Device* device)
{
  if (!device) return NULL;
  return (CompactFlashDevice*)device->data;
}

static void destroyCompactFlashDevice(HBC56Device *device)
{
  CompactFlashDevice *cfDevice = getCompactFlashDevice(device);
  CF_Destroy(cfDevice->compactFlash);
  free(cfDevice);
  device->data = NULL;
}

static uint8_t readCompactFlashDevice(HBC56Device* device, uint16_t addr, uint8_t *val, uint8_t dbg)
{
  const CompactFlashDevice* cfDevice = getCompactFlashDevice(device);
  uint16_t offset = addr - cfDevice->startAddr;

  switch(offset) {
    case CF_STAT:
    *val |= CF_Read_Status_Error(cfDevice->compactFlash) << 0;
    *val |= CF_Read_Status_CorrectableDataError(cfDevice->compactFlash) << 2;
    *val |= CF_Read_Status_DataRequest(cfDevice->compactFlash) << 3;
    *val |= CF_Read_Status_MemoryCardReady(cfDevice->compactFlash) << 4;
    *val |= CF_Read_Status_DriveWriteFault(cfDevice->compactFlash) << 5;
    *val |= CF_Read_Status_Ready(cfDevice->compactFlash) << 6;
    *val |= CF_Read_Status_Busy(cfDevice->compactFlash) << 7;
    break;

    case CF_ERR:
    *val |= CF_Read_Error_BadBlock(cfDevice->compactFlash) << 7;
    *val |= CF_Read_Error_UncorrectableError(cfDevice->compactFlash) << 6;
    *val |= CF_Read_Error_InvalidSector(cfDevice->compactFlash) << 4;
    *val |= CF_Read_Error_InvalidCommand(cfDevice->compactFlash) << 2;
    *val |= CF_Read_Error_GeneralError(cfDevice->compactFlash) << 0;
    break;

    case CF_DATA:
    *val = CF_Read_Data(cfDevice->compactFlash, dbg);
    break;
  }

  return 1;
}

static uint8_t writeCompactFlashDevice(HBC56Device* device, uint16_t addr, uint8_t val)
{
    const CompactFlashDevice* cfDevice = getCompactFlashDevice(device);
    uint8_t offset = addr - cfDevice->startAddr;
    
    switch(offset) {
      case CF_SECCO:
      CF_Write_SectorCount(cfDevice->compactFlash, val);
      break;

      case CF_LBA0:
      CF_Write_SectorNumber(cfDevice->compactFlash,
        (CF_Read_SectorNumber(cfDevice->compactFlash) & 0xFFFFFF00) | val
      );
      break;

      case CF_LBA1:
      CF_Write_SectorNumber(cfDevice->compactFlash,
        (CF_Read_SectorNumber(cfDevice->compactFlash) & 0xFFFF00FF) | (val << 8)
      );
      break;

      case CF_LBA2:
      CF_Write_SectorNumber(cfDevice->compactFlash,
        (CF_Read_SectorNumber(cfDevice->compactFlash) & 0xFF00FFFF) | (val << 16)
      );
      break;

      case CF_LBA3:
      CF_Write_SectorNumber(cfDevice->compactFlash,
        (CF_Read_SectorNumber(cfDevice->compactFlash) & 0x00FFFFFF) | ((val << 24) & 0x1F000000)
      );
      break;

      case CF_CMD:
      switch(val) {
        case CF_Command_ReadSectors:
        CF_Write_Command_ReadSectors(cfDevice->compactFlash);
        break;
      }
      break;

    }
    return 1;
}