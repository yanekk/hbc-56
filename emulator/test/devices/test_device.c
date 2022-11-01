#include "acutest.h"
#include "devices/device.h"

#include <strings.h>
#include <stdint.h>
#include <SDL2/SDL.h>

static HBC56Device createTestDevice() {
    return createDevice("my_device");
}

void test_createDevice_nameIsSet(void)
{
    const HBC56Device test_device = createDevice("my_device");
    TEST_ASSERT(strcmp(test_device.name, "my_device") == 0);
}

void test_createDevice_resetFnIsNull(void)
{
    TEST_ASSERT(createTestDevice().resetFn == NULL);
}

void test_createDevice_destroyFnIsNull(void)
{
    TEST_ASSERT(createTestDevice().destroyFn == NULL);
}

void test_createDevice_tickFnIsNull(void)
{
    TEST_ASSERT(createTestDevice().tickFn == NULL);
}

void test_createDevice_readFnIsNull(void)
{
    TEST_ASSERT(createTestDevice().readFn == NULL);
}

void test_createDevice_writeFnIsNull(void)
{
    TEST_ASSERT(createTestDevice().writeFn == NULL);
}

void test_createDevice_renderFnIsNull(void)
{
    TEST_ASSERT(createTestDevice().renderFn == NULL);
}

void test_createDevice_audioFnIsNull(void)
{
    TEST_ASSERT(createTestDevice().audioFn == NULL);
}

void test_createDevice_eventFnIsNull(void)
{
    TEST_ASSERT(createTestDevice().eventFn == NULL);
}

void test_createDevice_outputIsNull(void)
{
    TEST_ASSERT(createTestDevice().output == NULL);
}

void test_createDevice_dataIsNull(void)
{
    TEST_ASSERT(createTestDevice().data == NULL);
}

void test_createDevice_visibleIsTrue(void)
{
    TEST_ASSERT(createTestDevice().visible == true);
}

static bool isDeviceDestroyed = false;
void destroyTestDevice(HBC56Device * device) {
    isDeviceDestroyed = true;
}

void test_destroyDevice_destroyFnIsCalled(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.destroyFn = destroyTestDevice;

    // act
    destroyDevice(&testDevice);

    // assert
    TEST_ASSERT(isDeviceDestroyed == true);
}

void test_destroyDevice_dataIsFreed(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.data = (void*)malloc((size_t)16);

    // act
    destroyDevice(&testDevice);

    // assert
    TEST_ASSERT(testDevice.data == NULL);
}

void test_destroyDevice_deviceMemoryIsCleared(void)
{
    // arrange
    HBC56Device testDevice = createDevice("my_device");

    // act
    destroyDevice(&testDevice);

    // assert
    TEST_ASSERT(testDevice.name == NULL);
}

static bool isDeviceReset = false;
void resetTestDevice(HBC56Device * device) {
    isDeviceReset = true;
}

void test_resetDevice_resetFnIsCalled(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.resetFn = resetTestDevice;

    // act
    resetDevice(&testDevice);

    // assert
    TEST_ASSERT(isDeviceReset == true);
}

void test_resetDevice_resetFnIsNotCalledIfNotDefined(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();

    // act
    resetDevice(&testDevice);

    // assert
    TEST_ASSERT(true);
}

void test_resetDevice_resetFnIsNotCalledIfNull(void)
{
    // act
    resetDevice(NULL);

    // assert
    TEST_ASSERT(true);
}

static bool isDeviceTicked = false;
static uint32_t deltaTicksArg = 0;
static double deltaTimeArg = 0;

void tickTestDevice(HBC56Device * device, uint32_t deltaTicks, double deltaTime) {
    isDeviceTicked = true;
    deltaTicksArg = deltaTicks;
    deltaTimeArg = deltaTime;
}

void test_tickDevice_tickFnIsCalled(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.tickFn = tickTestDevice;

    // act
    tickDevice(&testDevice, 100, 1);

    // assert
    TEST_ASSERT(isDeviceTicked == true);
    TEST_ASSERT(deltaTicksArg == 100);
    TEST_ASSERT(deltaTimeArg == 1);
}

void test_tickDevice_tickFnIsNotCalledIfNotDefined(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();

    // act
    tickDevice(&testDevice, 0, 0);

    // assert
    TEST_ASSERT(true);
}

void test_tickDevice_tickFnIsNotCalledIfNull(void)
{
    // act
    tickDevice(NULL, 0, 0);

    // assert
    TEST_ASSERT(true);
}

static bool isDeviceRead = false;
static uint16_t addrReceived = 0;
static uint8_t valReceived = 0;

uint8_t readTestDevice(HBC56Device * device, uint16_t addr, uint8_t* val, uint8_t dbg) {
    isDeviceRead = true;
    addrReceived = addr;
    valReceived = *val;
    return 1;
}

void test_readDevice_readFnIsCalled(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.readFn = readTestDevice;
    uint8_t testValue = 0;

    // act
    readDevice(&testDevice, 100, &testValue, 0);

    // assert
    TEST_ASSERT(isDeviceRead == true);
}

void test_readDevice_readFnReturnsValue(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.readFn = readTestDevice;
    uint8_t testValue = 0;

    // act
    uint8_t returnValue = readDevice(&testDevice, 100, &testValue, 0);

    // assert
    TEST_ASSERT(returnValue == 1);
}

void test_readDevice_addressIsPasedToReadFn(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.readFn = readTestDevice;
    uint8_t testValue = 0;

    // act
    readDevice(&testDevice, 24, &testValue, 0);

    // assert
    TEST_ASSERT(addrReceived == 24);
}

void test_readDevice_valueIsPassedToVal(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.readFn = readTestDevice;
    uint8_t testValue = 15;

    // act
    readDevice(&testDevice, 0, &testValue, 0);

    // assert
    TEST_ASSERT(valReceived == 15);
}

void test_readDevice_readFnIsNotCalledIfNotDefined(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();

    // act
    readDevice(&testDevice, 0, 0, 0);

    // assert
    TEST_ASSERT(true);
}

void test_readDevice_readFnIsNotCalledIfNull(void)
{
    // act
    readDevice(NULL, 0, 0, 0);

    // assert
    TEST_ASSERT(true);
}

static bool isDeviceWritten = false;
static uint16_t writeAddrReceived = 0;
static uint8_t writeValReceived = 0;

uint8_t writeTestDevice(HBC56Device * device, uint16_t addr, uint8_t val) {
    isDeviceWritten = true;
    writeAddrReceived = addr;
    writeValReceived = val;
    return 1;
}

void test_writeDevice_writeFnIsCalled(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.writeFn = writeTestDevice;

    // act
    writeDevice(&testDevice, 0, 0);

    // assert
    TEST_ASSERT(isDeviceWritten == true);
}

void test_writeDevice_writeFnReturnsValue(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.writeFn = writeTestDevice;

    // act
    uint8_t returnValue = writeDevice(&testDevice, 0, 0);

    // assert
    TEST_ASSERT(returnValue == 1);
}

void test_writeDevice_addressIsPasedToWriteFn(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.writeFn = writeTestDevice;

    // act
    writeDevice(&testDevice, 36, 0);

    // assert
    TEST_ASSERT(writeAddrReceived == 36);
}

void test_writeDevice_valueIsPassedToVal(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.writeFn = writeTestDevice;

    // act
    writeDevice(&testDevice, 0, 17);

    // assert
    TEST_ASSERT(writeValReceived == 17);
}

void test_writeDevice_writeFnIsNotCalledIfNotDefined(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();

    // act
    writeDevice(&testDevice, 0, 0);

    // assert
    TEST_ASSERT(true);
}

void test_writeDevice_writeFnIsNotCalledIfNull(void)
{
    // act
    writeDevice(NULL, 0, 0);

    // assert
    TEST_ASSERT(true);
}

static bool isDeviceRendered = false;

void renderTestDevice(HBC56Device * device) {
    isDeviceRendered = true;
}

void test_renderDevice_renderFnIsCalled(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.renderFn = renderTestDevice;

    // act
    renderDevice(&testDevice);

    // assert
    TEST_ASSERT(isDeviceRendered == true);
}

void test_renderDevice_renderFnIsNotCalledIfNotDefined(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();

    // act
    renderDevice(&testDevice);

    // assert
    TEST_ASSERT(true);
}

void test_renderDevice_renderFnIsNotCalledIfNull(void)
{
    // act
    renderDevice(NULL);

    // assert
    TEST_ASSERT(true);
}

static bool isDeviceAudioRendered = false;
static float* bufferReceived = 0;
static int numSamplesReceived = 0;

void audioTestDevice(HBC56Device * device, float* buffer, int numSamples) {
    isDeviceAudioRendered = true;
    bufferReceived = buffer;
    numSamplesReceived = numSamples;
}

void test_renderAudioDevice_audioFnIsCalled(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.audioFn = audioTestDevice;

    // act
    renderAudioDevice(&testDevice, 0, 0);

    // assert
    TEST_ASSERT(isDeviceAudioRendered == true);
}

void test_renderAudioDevice_bufferIsPassedToAudioFn(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.audioFn = audioTestDevice;
    float testValue = 52;

    // act
    renderAudioDevice(&testDevice, &testValue, 0);

    // assert
    TEST_ASSERT(*bufferReceived == 52);
}

void test_renderAudioDevice_numSamplesIsPassedToVal(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.audioFn = audioTestDevice;

    // act
    renderAudioDevice(&testDevice, 0, 61);

    // assert
    TEST_ASSERT(numSamplesReceived == 61);
}

void test_renderAudioDevice_AudioFnIsNotCalledIfNotDefined(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();

    // act
    renderAudioDevice(&testDevice, 0, 0);

    // assert
    TEST_ASSERT(true);
}

void test_renderAudioDevice_audioFnIsNotCalledIfNull(void)
{
    // act
    renderAudioDevice(NULL, 0, 0);

    // assert
    TEST_ASSERT(true);
}

static bool hasDeviceEventFired = false;
static SDL_Event* receivedEvent = NULL;

void eventTestDevice(HBC56Device * device, SDL_Event* event) {
    hasDeviceEventFired = true;
    receivedEvent = event;
}

void test_eventDevice_eventFnIsCalled(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.eventFn = eventTestDevice;

    // act
    eventDevice(&testDevice, NULL);

    // assert
    TEST_ASSERT(hasDeviceEventFired == true);
}

void test_eventDevice_eventIsPassedToEventFn(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();
    testDevice.eventFn = eventTestDevice;
    SDL_Event testEvent = (SDL_Event)(SDL_WindowEvent){
        .type = SDL_WINDOWEVENT,
        .timestamp = 1234567
    };

    // act
    eventDevice(&testDevice, &testEvent);

    // assert
    TEST_ASSERT(receivedEvent == &testEvent);
}

void test_eventDevice_eventFnIsNotCalledIfNotDefined(void)
{
    // arrange
    HBC56Device testDevice = createTestDevice();

    // act
    eventDevice(&testDevice, NULL);

    // assert
    TEST_ASSERT(true);
}

void test_eventDevice_eventFnIsNotCalledIfNull(void)
{
    // act
    eventDevice(NULL, NULL);

    // assert
    TEST_ASSERT(true);
}

TEST_LIST = {
   { "test_createDevice_nameIsSet", test_createDevice_nameIsSet },
   { "test_createDevice_resetFnIsNull", test_createDevice_resetFnIsNull },
   { "test_createDevice_destroyFnIsNull", test_createDevice_destroyFnIsNull },
   { "test_createDevice_tickFnIsNull", test_createDevice_tickFnIsNull },
   { "test_createDevice_readFnIsNull", test_createDevice_readFnIsNull },
   { "test_createDevice_writeFnIsNull", test_createDevice_writeFnIsNull },
   { "test_createDevice_renderFnIsNull", test_createDevice_renderFnIsNull },
   { "test_createDevice_audioFnIsNull", test_createDevice_audioFnIsNull },
   { "test_createDevice_eventFnIsNull", test_createDevice_eventFnIsNull },
   { "test_createDevice_outputIsNull", test_createDevice_outputIsNull },
   { "test_createDevice_dataIsNull", test_createDevice_dataIsNull },
   { "test_createDevice_visibleIsTrue", test_createDevice_visibleIsTrue },
   
   { "test_destroyDevice_destroyFnIsCalled", test_destroyDevice_destroyFnIsCalled },
   { "test_destroyDevice_dataIsFreed", test_destroyDevice_dataIsFreed },
   { "test_destroyDevice_deviceMemoryIsCleared", test_destroyDevice_deviceMemoryIsCleared },
   
   { "test_resetDevice_resetFnIsCalled", test_resetDevice_resetFnIsCalled },
   { "test_resetDevice_resetFnIsNotCalledIfNotDefined", test_resetDevice_resetFnIsNotCalledIfNotDefined },
   { "test_resetDevice_resetFnIsNotCalledIfNull", test_resetDevice_resetFnIsNotCalledIfNull },
   { "test_resetDevice_tickFnIsCalled", test_tickDevice_tickFnIsCalled },
   { "test_resetDevice_tickFnIsNotCalledIfNotDefined", test_tickDevice_tickFnIsNotCalledIfNotDefined },
   
   { "test_readDevice_readFnIsCalled", test_readDevice_readFnIsCalled },
   { "test_readDevice_readFnReturnsValue", test_readDevice_readFnReturnsValue },
   { "test_readDevice_addressIsPasedToReadFn", test_readDevice_addressIsPasedToReadFn },
   { "test_readDevice_valueIsPassedToVal", test_readDevice_valueIsPassedToVal },
   { "test_readDevice_readFnIsNotCalledIfNotDefined", test_readDevice_readFnIsNotCalledIfNotDefined },
   { "test_readDevice_readFnIsNotCalledIfNull", test_readDevice_readFnIsNotCalledIfNull },
   
   { "test_writeDevice_writeFnIsCalled", test_writeDevice_writeFnIsCalled },
   { "test_writeDevice_writeFnReturnsValue", test_writeDevice_writeFnReturnsValue },
   { "test_writeDevice_addressIsPasedToWriteFn", test_writeDevice_addressIsPasedToWriteFn },
   { "test_writeDevice_valueIsPassedToVal", test_writeDevice_valueIsPassedToVal },
   { "test_writeDevice_writeFnIsNotCalledIfNotDefined", test_writeDevice_writeFnIsNotCalledIfNotDefined },
   { "test_writeDevice_writeFnIsNotCalledIfNull", test_writeDevice_writeFnIsNotCalledIfNull },
   
   { "test_renderDevice_renderFnIsCalled", test_renderDevice_renderFnIsCalled },
   { "test_renderDevice_renderFnIsNotCalledIfNotDefined", test_renderDevice_renderFnIsNotCalledIfNotDefined },
   { "test_renderDevice_renderFnIsNotCalledIfNull", test_renderDevice_renderFnIsNotCalledIfNull },
   
   { "test_renderAudioDevice_audioFnIsCalled", test_renderAudioDevice_audioFnIsCalled },
   { "test_renderAudioDevice_bufferIsPassedToAudioFn", test_renderAudioDevice_bufferIsPassedToAudioFn },
   { "test_renderAudioDevice_numSamplesIsPassedToVal", test_renderAudioDevice_numSamplesIsPassedToVal },
   { "test_renderAudioDevice_AudioFnIsNotCalledIfNotDefined", test_renderAudioDevice_AudioFnIsNotCalledIfNotDefined },
   { "test_renderAudioDevice_audioFnIsNotCalledIfNull", test_renderAudioDevice_audioFnIsNotCalledIfNull },

   { "test_eventDevice_eventFnIsCalled", test_eventDevice_eventFnIsCalled },
   { "test_eventDevice_eventIsPassedToEventFn", test_eventDevice_eventIsPassedToEventFn },
   { "test_eventDevice_eventFnIsNotCalledIfNotDefined", test_eventDevice_eventFnIsNotCalledIfNotDefined },
   { "test_eventDevice_eventFnIsNotCalledIfNull", test_eventDevice_eventFnIsNotCalledIfNull },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};