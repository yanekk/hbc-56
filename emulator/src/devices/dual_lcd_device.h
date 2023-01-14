#ifndef _HBC56_DUAL_LCD_DEVICE_H_
#define _HBC56_DUAL_LCD_DEVICE_H_

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_CMD_MASK                0b11000000
#define LCD_CMD_DISPLAY_ON_OFF_MASK 0b00000000
#define LCD_CMD_SET_ADDRESS_MASK    0b01000000 // bits 0:5 Y address
#define LCD_CMD_SET_PAGE_MASK       0b10000000 // bits 0:2 X address
#define LCD_CMD_SET_START_LINE_MASK 0b11000000 // bits 0:5 start line

#define LCD_CMD_DISPLAY_ON_OFF_VALUE_MASK 0b00000001
#define LCD_CMD_SET_ADDRESS_VALUE_MASK 0b00111111

HBC56Device createDualLcdDevice(uint16_t segmentAAddress, uint16_t segmentBAddress);

#ifdef __cplusplus
}
#endif

#endif