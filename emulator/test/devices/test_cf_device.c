#include "acutest.h"
#include "devices/cf_device.h"
#include "devices/device.h"
#include "devices/compactflash/compactflash.h"

#define BASE_ADDRESS 0x1000

typedef struct CompactFlashSpy {
    bool isCreated,
         isDestroyed;

    bool statusError,
         statusCorrectableDataError,
         statusDataRequest,
         statusMemoryCardReady,
         statusDriveWriteFault,
         statusReady,
         statusBusy;

    bool errorBadBlock, 
         errorUncorrectableError,
         errorInvalidSector,
         errorInvalidCommand,
         errorGeneralError;

    uint32_t sectorNumber;

    uint8_t sectorCount;
    uint8_t * data;
    uint8_t dataByte;
    bool isDebugger;
    uint8_t executedCommandCode;
} CompactFlashSpy;

CompactFlashSpy compactFlashSpy = {};
CompactFlash testCfCard = {};

HBC56Device testDevice;

CompactFlash* CF_Create(const uint8_t *data) {
    compactFlashSpy.isCreated = true;
    compactFlashSpy.data = (uint8_t*)data;
    return &testCfCard;
}

bool CF_Read_Status_Busy(CompactFlash *device) {
    return compactFlashSpy.statusBusy;
}

bool CF_Read_Status_Ready(CompactFlash *device) {
    return compactFlashSpy.statusReady;
}

bool CF_Read_Status_DriveWriteFault(CompactFlash *device) {
    return compactFlashSpy.statusDriveWriteFault;
}

bool CF_Read_Status_MemoryCardReady(CompactFlash *device) {
    return compactFlashSpy.statusMemoryCardReady;
}

bool CF_Read_Status_DataRequest(CompactFlash *device) {
    return compactFlashSpy.statusDataRequest;
}

bool CF_Read_Status_CorrectableDataError(CompactFlash *device) {
    return compactFlashSpy.statusCorrectableDataError;
}

bool CF_Read_Status_Error(CompactFlash *device) {
    return compactFlashSpy.statusError;
}

bool CF_Read_Error_BadBlock(CompactFlash *device) {
    return compactFlashSpy.errorBadBlock;
}

bool CF_Read_Error_UncorrectableError(CompactFlash *device) {
    return compactFlashSpy.errorUncorrectableError;
}

bool CF_Read_Error_InvalidSector(CompactFlash *device) {
    return compactFlashSpy.errorInvalidSector;
}

bool CF_Read_Error_InvalidCommand(CompactFlash *device) {
    return compactFlashSpy.errorInvalidCommand;
}

bool CF_Read_Error_GeneralError(CompactFlash *device) {
    return compactFlashSpy.errorGeneralError;
}

void CF_Destroy(CompactFlash * device) {
      compactFlashSpy.isDestroyed = true;
}

void CF_Write_SectorCount(CompactFlash *device, uint8_t sectorCount) {
    compactFlashSpy.sectorCount = sectorCount;
}

uint32_t CF_Read_SectorNumber(CompactFlash *device) {
    return compactFlashSpy.sectorNumber;
}

void CF_Write_SectorNumber(CompactFlash *device, uint32_t number) {
    compactFlashSpy.sectorNumber = number;
}

void CF_Write_Command_ReadSectors(CompactFlash *device) {
    compactFlashSpy.executedCommandCode = CF_Command_ReadSectors;
}

uint8_t CF_Read_Data(CompactFlash *device, bool isDebugger) {
    compactFlashSpy.isDebugger = isDebugger;
    return compactFlashSpy.dataByte;
}


void testInit() {
    compactFlashSpy = (CompactFlashSpy){0};
    testDevice = createCompactFlashDevice(BASE_ADDRESS, NULL);
}

uint8_t testRead(uint8_t offset) {
    uint8_t value = 0;
    readDevice(&testDevice, BASE_ADDRESS + offset, &value, true);
    return value;
}

void testWrite(uint8_t offset, uint8_t value) {
    writeDevice(&testDevice, BASE_ADDRESS + offset, value);
}

void test_createDevice_nameIsSet(void)
{
    // arrange
    compactFlashSpy = (CompactFlashSpy){0};

    // act
    HBC56Device testDevice = createCompactFlashDevice(0, NULL);

    // assert 
    TEST_ASSERT(strcmp(testDevice.name, "CompactFlash") == 0);
}

void test_createDevice_cfCardIsCreated(void)
{
    // arrange
    compactFlashSpy = (CompactFlashSpy){0};

    // act
    createCompactFlashDevice(0, NULL);

    // assert 
    TEST_ASSERT(compactFlashSpy.isCreated == true);
}

void test_createDevice_dataIsPassedToCfCard(void)
{
    // arrange
    compactFlashSpy = (CompactFlashSpy){0};
    uint8_t data[] = {0, 1, 2, 3, 4};

    // act
    createCompactFlashDevice(0, data);

    // assert 
    TEST_ASSERT(compactFlashSpy.data == data);
}

void test_destroyDevice_cfCardIsDestroyed(void)
{
    // arrange
    testInit(); 

    // act
    destroyDevice(&testDevice);

    // assert 
    TEST_ASSERT(compactFlashSpy.isDestroyed == true);
}

void test_readDevice_CF_STAT_zeroByDefault(void)
{
    // arrange
    testInit(); 
    
    // act
    uint8_t actualStatus = testRead(CF_STAT);

    // assert
    TEST_ASSERT(actualStatus ==  0b00000000);
}

void test_readDevice_CF_STAT_statusErrorSetBit0(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.statusError = true;

    // act & assert
    TEST_ASSERT(testRead(CF_STAT) == 0b00000001);
}

void test_readDevice_CF_STAT_statusCorrectableDataErrorSetBit2(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.statusCorrectableDataError = true;

    // act & assert
    TEST_ASSERT(testRead(CF_STAT) == 0b00000100);
}

void test_readDevice_CF_STAT_statusDataRequestSetBit3(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.statusDataRequest = true;

    // act & assert
    TEST_ASSERT(testRead(CF_STAT) == 0b00001000);
}

void test_readDevice_CF_STAT_statusMemoryCardReadySetBit4(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.statusMemoryCardReady = true;

    // act & assert
    TEST_ASSERT(testRead(CF_STAT) == 0b00010000);
}

void test_readDevice_CF_STAT_statusDriveWriteFaultSetBit5(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.statusDriveWriteFault = true;

    // act & assert
    TEST_ASSERT(testRead(CF_STAT) == 0b00100000);
}

void test_readDevice_CF_STAT_statusReadySetBit6(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.statusReady = true;

    // act & assert
    TEST_ASSERT(testRead(CF_STAT) == 0b01000000);
}

void test_readDevice_CF_STAT_statusBusySetBit7(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.statusBusy = true;

    // act & assert
    TEST_ASSERT(testRead(CF_STAT) == 0b10000000);
}

void test_readDevice_CF_ERR_errorBadBlockSetBit7(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.errorBadBlock = true;

    // act & assert
    TEST_ASSERT(testRead(CF_ERR) == 0b10000000);
}

void test_readDevice_CF_ERR_errorUncorrectableErrorSetBit6(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.errorUncorrectableError = true;

    // act & assert
    TEST_ASSERT(testRead(CF_ERR) == 0b01000000);
}

void test_readDevice_CF_ERR_errorInvalidSectorSetBit4(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.errorInvalidSector = true;

    // act & assert
    TEST_ASSERT(testRead(CF_ERR) == 0b00010000);
}

void test_readDevice_CF_ERR_errorInvalidCommandSetBit2(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.errorInvalidCommand = true;

    // act & assert
    TEST_ASSERT(testRead(CF_ERR) == 0b00000100);
}

void test_readDevice_CF_ERR_errorGeneralErrorSetBit0(void)
{
    // arrange
    testInit(); 
    compactFlashSpy.errorGeneralError = true;

    // act & assert
    TEST_ASSERT(testRead(CF_ERR) == 0b00000001);
}

void test_writeDevice_CF_SECCO_sectorCountIsSet(void)
{
    // arrange
    testInit();
    uint8_t expectedSectorCount = 6;

    testWrite(CF_SECCO, expectedSectorCount);

    // act & assert
    TEST_ASSERT(compactFlashSpy.sectorCount == expectedSectorCount);
}

void test_writeDevice_CFLBA0_first0_7OfSectorNumberAreSet(void)
{
    // arrange
    testInit();
    testWrite(CF_LBA0, 0xAF);

    // act & assert
    TEST_ASSERT(compactFlashSpy.sectorNumber == 0x000000AF);
}

void test_writeDevice_CFLBA0_bits0_7OfSectorNumberAreOverwritten(void)
{
    // arrange
    testInit();
    compactFlashSpy.sectorNumber = 0x000000FF;
    testWrite(CF_LBA0,  0x01);

    // act & assert
    TEST_ASSERT(compactFlashSpy.sectorNumber == 0x00000001);
}

void test_writeDevice_CFLBA1_bits8_15bitsOfSectorNumberAreSet(void)
{
    // arrange
    testInit();
    testWrite(CF_LBA1, 0xAF);

    // act & assert
    TEST_ASSERT(compactFlashSpy.sectorNumber == 0x0000AF00);
}

void test_writeDevice_CFLBA1_bits8_15OfSectorNumberAreOverwritten(void)
{
    // arrange
    testInit();
    compactFlashSpy.sectorNumber = 0x0000FF00;
    testWrite(CF_LBA1, 0x01);

    // act & assert
    TEST_ASSERT(compactFlashSpy.sectorNumber == 0x00000100);
}

void test_writeDevice_CFLBA2_bits16_23bitsOfSectorNumberAreSet(void)
{
    // arrange
    testInit();
    testWrite(CF_LBA2, 0xAF);

    // act & assert
    TEST_ASSERT(compactFlashSpy.sectorNumber == 0x00AF0000);
}

void test_writeDevice_CFLBA2_bits16_23OfSectorNumberAreOverwritten(void)
{
    // arrange
    testInit();
    compactFlashSpy.sectorNumber = 0x0000FF00;
    testWrite(CF_LBA2, 0x01);

    // act & assert
    TEST_ASSERT(compactFlashSpy.sectorNumber == 0x00010000);
}

void test_writeDevice_CFLBA3_bits24_27bitsOfSectorNumberAreSet(void)
{
    // arrange
    testInit();
    testWrite(CF_LBA3, 0xFF);

    // act & assert
    TEST_ASSERT(compactFlashSpy.sectorNumber == 0x1F000000);
}

void test_writeDevice_CFLBA3_bits24_27OfSectorNumberAreOverwritten(void)
{
    // arrange
    testInit();
    compactFlashSpy.sectorNumber = 0x01000000;
    testWrite(CF_LBA3, 0x01);

    // act & assert
    TEST_ASSERT(compactFlashSpy.sectorNumber == 0x01000000);
}

void test_writeDevice_CFLBAX_allBitsAreSet(void)
{
    // arrange
    testInit();
    compactFlashSpy.sectorNumber = 0x01000000;
    
    testWrite(CF_LBA3, 0x01);
    testWrite(CF_LBA2, 0x23);
    testWrite(CF_LBA1, 0x45);
    testWrite(CF_LBA0, 0x67);

    // act & assert
    TEST_ASSERT(compactFlashSpy.sectorNumber == 0x01234567);
}

void test_writeDevice_DATA_dataIsReturned(void)
{
    // arrange
    testInit();
    compactFlashSpy.dataByte = 0xFA;
    
    // act & assert
    TEST_ASSERT(testRead(CF_DATA) == 0xFA);
}

void test_writeDevice_DATA_debugFlagIsPased(void)
{
    // arrange
    testInit();
    compactFlashSpy.dataByte = 0xFA;
    testRead(CF_DATA);

    // act & assert
    TEST_ASSERT(compactFlashSpy.isDebugger == true);
}

void test_writeDevice_CMD_ReadSectors(void)
{
    // arrange
    testInit();

    testWrite(CF_CMD, CF_Command_ReadSectors);

    // act & assert
    TEST_ASSERT(compactFlashSpy.executedCommandCode == CF_Command_ReadSectors);
}

TEST_LIST = {
   { "test_createDevice_nameIsSet", test_createDevice_nameIsSet },
   { "test_createDevice_cfCardIsCreated", test_createDevice_cfCardIsCreated },
   { "test_createDevice_dataIsPassedToCfCard", test_createDevice_dataIsPassedToCfCard },
   { "test_destroyDevice_cfCardIsDestroyed", test_destroyDevice_cfCardIsDestroyed },
   { "test_readDevice_CF_STAT_zeroByDefault", test_readDevice_CF_STAT_zeroByDefault },
   { "test_readDevice_CF_STAT_statusErrorSetBit0", test_readDevice_CF_STAT_statusErrorSetBit0 },
   { "test_readDevice_CF_STAT_statusCorrectableDataErrorSetBit2", test_readDevice_CF_STAT_statusCorrectableDataErrorSetBit2 },
   { "test_readDevice_CF_STAT_statusDataRequestSetBit3", test_readDevice_CF_STAT_statusDataRequestSetBit3 },
   { "test_readDevice_CF_STAT_statusMemoryCardReadySetBit4", test_readDevice_CF_STAT_statusMemoryCardReadySetBit4 },
   { "test_readDevice_CF_STAT_statusReadySetBit6", test_readDevice_CF_STAT_statusReadySetBit6 },
   { "test_readDevice_CF_STAT_statusBusySetBit7", test_readDevice_CF_STAT_statusBusySetBit7 },
   { "test_readDevice_CF_ERR_errorBadBlockSetBit7", test_readDevice_CF_ERR_errorBadBlockSetBit7 },
   { "test_readDevice_CF_ERR_errorUncorrectableErrorSetBit6", test_readDevice_CF_ERR_errorUncorrectableErrorSetBit6 },
   { "test_readDevice_CF_ERR_errorInvalidSectorSetBit4", test_readDevice_CF_ERR_errorInvalidSectorSetBit4 },
   { "test_readDevice_CF_ERR_errorInvalidCommandSetBit2", test_readDevice_CF_ERR_errorInvalidCommandSetBit2 },
   { "test_readDevice_CF_ERR_errorGeneralErrorSetBit0", test_readDevice_CF_ERR_errorGeneralErrorSetBit0 },
   { "test_writeDevice_CF_SECCO_sectorCountIsSet", test_writeDevice_CF_SECCO_sectorCountIsSet },
   { "test_writeDevice_CFLBA0_first0_7OfSectorNumberAreSet", test_writeDevice_CFLBA0_first0_7OfSectorNumberAreSet },
   { "test_writeDevice_CFLBA0_bits0_7OfSectorNumberAreOverwritten", test_writeDevice_CFLBA0_bits0_7OfSectorNumberAreOverwritten },
   { "test_writeDevice_CFLBA1_bits8_15bitsOfSectorNumberAreSet", test_writeDevice_CFLBA1_bits8_15bitsOfSectorNumberAreSet },
   { "test_writeDevice_CFLBA1_bits8_15OfSectorNumberAreOverwritten", test_writeDevice_CFLBA1_bits8_15OfSectorNumberAreOverwritten },
   { "test_writeDevice_CFLBA2_bits16_23bitsOfSectorNumberAreSet", test_writeDevice_CFLBA2_bits16_23bitsOfSectorNumberAreSet },
   { "test_writeDevice_CFLBA2_bits16_23OfSectorNumberAreOverwritten", test_writeDevice_CFLBA2_bits16_23bitsOfSectorNumberAreSet },
   { "test_writeDevice_CFLBA3_bits24_27bitsOfSectorNumberAreSet", test_writeDevice_CFLBA3_bits24_27bitsOfSectorNumberAreSet },
   { "test_writeDevice_CFLBA3_bits24_27OfSectorNumberAreOverwritten", test_writeDevice_CFLBA3_bits24_27OfSectorNumberAreOverwritten },
   { "test_writeDevice_CFLBAX_allBitsAreSet", test_writeDevice_CFLBAX_allBitsAreSet },   
   { "test_writeDevice_DATA_dataIsReturned", test_writeDevice_DATA_dataIsReturned },    
   { "test_writeDevice_CMD_ReadSectors", test_writeDevice_CMD_ReadSectors },   
   { NULL, NULL }     /* zeroed record marking the end of the list */
};