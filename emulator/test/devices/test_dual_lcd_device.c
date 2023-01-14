#include "acutest.h"
#include "devices/dual_lcd_device.h"
#include "devices/device.h"
#include "devices/lcd/segment.h"
#include "devices/lcd/renderer.h"
#include "utils/matrix.h"

#define LCD_SEGMENT_A 0x4200
#define LCD_SEGMENT_B 0x4400

#define LCD_SEGMENT_A_CMD LCD_SEGMENT_A
#define LCD_SEGMENT_B_CMD LCD_SEGMENT_B

#define LCD_SEGMENT_A_DATA LCD_SEGMENT_A_CMD+1
#define LCD_SEGMENT_B_DATA LCD_SEGMENT_B_CMD+1

#define CMD_DISPLAY_ON     0b00111111
#define CMD_DISPLAY_OFF    0b00111110
#define CMD_SET_ADDRESS    0b01000000 // bits 0:5 Y address
#define CMD_SET_PAGE       0b10111000 // bits 0:2 X address
#define CMD_SET_START_LINE 0b11000000 // bits 0:5 start line

#define CMD_STATUS_BUSY_MASK 0b10000000 
#define CMD_STATUS_ON_OFF    0b00100000

LcdRendererImageData renderedData;
void LcdRenderer_Render(LcdRenderer* renderer, LcdRendererImageData* imageData) {
    memcpy(renderedData.data, imageData->data, sizeof(imageData->data));
}

void test_createDevice_nameIsSet(void)
{
    const HBC56Device testDevice = createDualLcdDevice(NULL, LCD_SEGMENT_A, LCD_SEGMENT_B);
    TEST_ASSERT(strcmp(testDevice.name, "Dual Graphics LCD") == 0);
}

HBC56Device lcdDevice;

void initTestDevice() {
    lcdDevice = createDualLcdDevice(NULL, LCD_SEGMENT_A, LCD_SEGMENT_B);
}

void writeTestDevice(uint16_t address, uint8_t data) {
    writeDevice(&lcdDevice, address, data);
}

uint8_t readTestDevice(uint16_t address) {
    uint8_t result;
    readDevice(&lcdDevice, address, &result, false);
    return result;
}

void test_writeDevice_turnOnSegmentA(void)
{
    // arrange
    initTestDevice();

    // act
    writeTestDevice(LCD_SEGMENT_A_CMD, CMD_DISPLAY_ON);

    // assert
    TEST_ASSERT((readTestDevice(LCD_SEGMENT_A_CMD) & CMD_STATUS_ON_OFF) != 0);
    TEST_ASSERT((readTestDevice(LCD_SEGMENT_B_CMD) & CMD_STATUS_ON_OFF) == 0);
}

void test_writeDevice_turnOnSegmentB(void)
{
    // arrange
    initTestDevice();

    // act
    writeTestDevice(LCD_SEGMENT_B_CMD, CMD_DISPLAY_ON);

    // assert
    TEST_ASSERT((readTestDevice(LCD_SEGMENT_A_CMD) & CMD_STATUS_ON_OFF) == 0);
    TEST_ASSERT((readTestDevice(LCD_SEGMENT_B_CMD) & CMD_STATUS_ON_OFF) != 0);
}

void test_writeDevice_turnOffSegmentA(void)
{
    // arrange
    initTestDevice();
    writeTestDevice(LCD_SEGMENT_A_CMD, CMD_DISPLAY_ON);
    writeTestDevice(LCD_SEGMENT_B_CMD, CMD_DISPLAY_ON);

    // act
    writeTestDevice(LCD_SEGMENT_A_CMD, CMD_DISPLAY_OFF);

    // assert
    TEST_ASSERT((readTestDevice(LCD_SEGMENT_A_CMD) & CMD_STATUS_ON_OFF) == 0);
    TEST_ASSERT((readTestDevice(LCD_SEGMENT_B_CMD) & CMD_STATUS_ON_OFF) != 0);
}

void test_writeDevice_turnOffSegmentB(void)
{
    // arrange
    initTestDevice();
    writeTestDevice(LCD_SEGMENT_A_CMD, CMD_DISPLAY_ON);
    writeTestDevice(LCD_SEGMENT_B_CMD, CMD_DISPLAY_ON);

    // act
    writeTestDevice(LCD_SEGMENT_B_CMD, CMD_DISPLAY_OFF);

    // assert
    TEST_ASSERT((readTestDevice(LCD_SEGMENT_A_CMD) & CMD_STATUS_ON_OFF) != 0);
    TEST_ASSERT((readTestDevice(LCD_SEGMENT_B_CMD) & CMD_STATUS_ON_OFF) == 0);
}

void test_writeDevice_dataCanBeWrittenAndRead()
{
    // arrange
    initTestDevice();
    writeTestDevice(LCD_SEGMENT_A_CMD, CMD_DISPLAY_ON);

    // act
    for(uint8_t y = 0; y < LCD_SEGMENT_COLUMNS; y++) {
        writeTestDevice(LCD_SEGMENT_A_DATA, 0xFF - y);
    }
    
    // assert
    for(uint8_t y = 0; y < LCD_SEGMENT_COLUMNS; y++) {
        TEST_ASSERT(readTestDevice(LCD_SEGMENT_A_DATA) == 0xFF - y);
    }
}

void test_setAddress_addressCanBeSet() {
    // arrange
    initTestDevice();
    writeTestDevice(LCD_SEGMENT_A_CMD, CMD_DISPLAY_ON);

    // act
    writeTestDevice(LCD_SEGMENT_A_CMD, LCD_CMD_SET_ADDRESS_MASK | 63);
    writeTestDevice(LCD_SEGMENT_A_DATA, 0xFA);
    writeTestDevice(LCD_SEGMENT_A_CMD, LCD_CMD_SET_ADDRESS_MASK | 63);

    // assert
    TEST_ASSERT(readTestDevice(LCD_SEGMENT_A_DATA) == 0xFA);
}

void test_setAddress_pageCanBeSet() {
    // arrange
    initTestDevice();
    writeTestDevice(LCD_SEGMENT_A_CMD, CMD_DISPLAY_ON);

    // act
    writeTestDevice(LCD_SEGMENT_A_CMD, LCD_CMD_SET_PAGE_MASK | 7);
    writeTestDevice(LCD_SEGMENT_A_DATA, 0xFA);
    

    // assert
    writeTestDevice(LCD_SEGMENT_A_CMD, LCD_CMD_SET_PAGE_MASK | 0);
    writeTestDevice(LCD_SEGMENT_A_CMD, LCD_CMD_SET_ADDRESS_MASK | 0);
    TEST_ASSERT(readTestDevice(LCD_SEGMENT_A_DATA) == 0);

    writeTestDevice(LCD_SEGMENT_A_CMD, LCD_CMD_SET_PAGE_MASK | 7);
    writeTestDevice(LCD_SEGMENT_A_CMD, LCD_CMD_SET_ADDRESS_MASK | 0);
    TEST_ASSERT(readTestDevice(LCD_SEGMENT_A_DATA) == 0xFA);
}

void test_setAddress_startLineCanBeSet() {
    // arrange
    initTestDevice();
    writeTestDevice(LCD_SEGMENT_A_CMD, CMD_DISPLAY_ON);
    writeTestDevice(LCD_SEGMENT_A_DATA, 0b11111111);
    writeTestDevice(LCD_SEGMENT_A_CMD, LCD_CMD_SET_START_LINE_MASK | 4);
    // act
    
    // assert
    renderDevice(&lcdDevice);

    uint8_t LINE_WIDTH = LCD_SEGMENT_COLUMNS * 2;
    
    TEST_ASSERT(renderedData.data[0] == 0);
    TEST_ASSERT(renderedData.data[LINE_WIDTH] == 0);
    TEST_ASSERT(renderedData.data[LINE_WIDTH*2] == 0);
    TEST_ASSERT(renderedData.data[LINE_WIDTH*3] == 0);
    TEST_ASSERT(renderedData.data[LINE_WIDTH*4] == 1);
    TEST_ASSERT(renderedData.data[LINE_WIDTH*5] == 1);
    TEST_ASSERT(renderedData.data[LINE_WIDTH*6] == 1);
    TEST_ASSERT(renderedData.data[LINE_WIDTH*7] == 1);
}

TEST_LIST = {
   { "test_createDevice_nameIsSet", test_createDevice_nameIsSet },
   { "test_writeDevice_turnOnSegmentA", test_writeDevice_turnOnSegmentA },
   { "test_writeDevice_turnOnSegmentB", test_writeDevice_turnOnSegmentB },
   { "test_writeDevice_turnOffSegmentA", test_writeDevice_turnOffSegmentA },
   { "test_writeDevice_turnOffSegmentB", test_writeDevice_turnOffSegmentB },
   { "test_writeDevice_dataCanBeWrittenAndRead", test_writeDevice_dataCanBeWrittenAndRead },
   { "test_setAddress_addressCanBeSet", test_setAddress_addressCanBeSet },
   { "test_setAddress_pageCanBeSet", test_setAddress_pageCanBeSet },
   { "test_setAddress_startLineCanBeSet", test_setAddress_startLineCanBeSet },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};