#include <stdlib.h>

#include "devices/device.h"
#include "devices/lcd/segment.h"
#include "devices/dual_lcd_device.h"

typedef struct {
    LcdSegment* segmentA;
    uint16_t segmentAAddress;
    
    LcdSegment* segmentB;
    uint16_t segmentBAddress;

} DualLcdDevice;

// static void resetDualLcdDevice(HBC56Device*);
static void destroyDualLcdDevice(HBC56Device*);
// static void renderDualLcdDevice(HBC56Device* device);
static uint8_t readDualLcdDevice(HBC56Device*, uint16_t, uint8_t*, uint8_t);
static uint8_t writeDualLcdDevice(HBC56Device*, uint16_t, uint8_t);

HBC56Device createDualLcdDevice(uint16_t segmentAAddress, uint16_t segmentBAddress) {
    HBC56Device device = createDevice("Dual Graphics LCD");

    DualLcdDevice* lcd = malloc(sizeof(DualLcdDevice));

    lcd->segmentA = LcdSegment_Create();
    lcd->segmentAAddress = segmentAAddress;

    lcd->segmentB = LcdSegment_Create();
    lcd->segmentBAddress = segmentBAddress;

    device.data = lcd;
    device.destroyFn = &destroyDualLcdDevice;
    // device.resetFn = &resetDualLcdDevice;
    device.readFn = &readDualLcdDevice;
    device.writeFn = &writeDualLcdDevice;
    //device.renderFn = &renderDualLcdDevice;

    return device;
}

static DualLcdDevice* getDualLcdDevice(HBC56Device* device) {
    if(device->data == NULL)
        return NULL;
    return (DualLcdDevice*)device->data;
}

static LcdSegment* getSegment(DualLcdDevice* lcdDevice, uint16_t address) {
    return (address & 0xFF00) == lcdDevice->segmentAAddress ? lcdDevice->segmentA : lcdDevice->segmentB;
}

static uint8_t readDualLcdDevice(HBC56Device* device, uint16_t address, uint8_t* data, uint8_t isDebug) {
    LcdSegment* segment = getSegment(getDualLcdDevice(device), address);

    if((address & 0x00FF) == 0) { // cmd
        *data = LcdSegment_State(segment) << 5;
        return 1;
    }
    
    if ((address & 0x00FF) == 1) { // data
        *data = LcdSegment_ReadData(segment, isDebug);
        return 1;
    }
    return 1;
}

static uint8_t sendCommand(LcdSegment* segment, uint8_t command) {
    uint8_t commandMask = command & LCD_CMD_MASK;
    switch (commandMask) {
    case LCD_CMD_DISPLAY_ON_OFF_MASK:
        if((command & LCD_CMD_DISPLAY_ON_OFF_VALUE_MASK) == 1) {
            LcdSegment_TurnOn(segment);
        } else {
            LcdSegment_TurnOff(segment);
        }
        break;
    }
    return 1;
}

static uint8_t writeDualLcdDevice(HBC56Device* device, uint16_t address, uint8_t data) {
    LcdSegment* segment = getSegment(getDualLcdDevice(device), address);
    if((address & 0x00FF) == 0)  // cmd
        return sendCommand(segment, data);

    if ((address & 0x00FF) == 1) 
        LcdSegment_WriteData(segment, data);
        return 1;

    return 1;
}

static void destroyDualLcdDevice(HBC56Device* device) {
    DualLcdDevice* lcdDevice = getDualLcdDevice(device);
    if(lcdDevice != NULL) {
        LcdSegment_Destroy(lcdDevice->segmentA);
        free(lcdDevice);
    }
    free(device);
}