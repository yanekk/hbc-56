#include "acutest.h"
#include "parseargs.h"
#include "devices/lcd_device.h"

#include <stdbool.h>
#include <string.h>

#define EXISTING_ROM_FILE "test/test_parseargs_data/correct_size_rom_file.bin"
#define EXISTING_CFCARD_FILE "test/test_parseargs_data/cfcard.bin"

char errorBuffer[1024];

void test_parsingFailsOnMissingArgv(void)
{
    // act
    Hbc56EmulatorArgs args = {0};
    bool ok = Hbc56EmulatorArgs_Parse(&args, 0, NULL, errorBuffer);

    // assert
    TEST_ASSERT(!ok);
    TEST_ASSERT(strcmp(errorBuffer, "Invalid arguments") == 0);
}

// todo: handle unknown args
void test_parsingRomFileNoErrorOnMissingRomOption(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {""};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 0, argv, errorBuffer);

    // assert
    TEST_ASSERT(!ok);
    TEST_ASSERT(strcmp(errorBuffer, "No HBC-56 ROM file.\n\nUse --rom <romfile>") == 0);
}

void test_parsingRomFileNoErrorOnExistingFile(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* romFile = EXISTING_ROM_FILE;
    char* argv[] = {"--rom", romFile};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 2, argv, errorBuffer);

    // assert
    TEST_ASSERT(ok);
    TEST_ASSERT(strcmp(args.romFile, romFile) == 0);
}

void test_parsingRomFileErrorOnNonExistingFile(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* romFile = "test/test_parseargs_data/non_existing_file.txt";
    char* argv[] = {"--rom", romFile};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 2, argv, errorBuffer);

    // assert
    TEST_ASSERT(!ok);
    TEST_ASSERT(args.romFile == NULL);
    TEST_ASSERT(strcmp(errorBuffer, "File 'test/test_parseargs_data/non_existing_file.txt' cannot be found.") == 0);
}

void test_parsingRomFileErrorOnIncorrectFileSize(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* romFile = "test/test_parseargs_data/incorrect_size_rom_file.bin";
    char* argv[] = {"--rom", romFile};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 2, argv, errorBuffer);

    // assert
    TEST_ASSERT(!ok);
    TEST_ASSERT(args.romFile == NULL);
    TEST_ASSERT(strcmp(errorBuffer, "ROM file 'test/test_parseargs_data/incorrect_size_rom_file.bin' has incorrect size. Expected: 32768, got 16384.") == 0);
}

void test_errorOnRomOptionAtTheEnd(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {"--rom"};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 1, argv, errorBuffer);

    // assert
    TEST_ASSERT(!ok);
    TEST_ASSERT(strcmp(errorBuffer, "No HBC-56 ROM file.\n\nUse --rom <romfile>") == 0);
}

void test_breakOnStartIsFalseByDefault(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {"--rom", EXISTING_ROM_FILE};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 3, argv, errorBuffer);

    // assert
    printf(errorBuffer);
    TEST_ASSERT(ok);
    TEST_ASSERT(args.breakOnStart == false);
}

void test_breakOnStartIsTrueWhenSet(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {"--rom", EXISTING_ROM_FILE, "--brk"};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 3, argv, errorBuffer);

    // assert
    TEST_ASSERT(ok);
    TEST_ASSERT(args.breakOnStart == true);
}

void test_lcdTypeIsNoneByDefault(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {"--rom", EXISTING_ROM_FILE};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 2, argv, errorBuffer);

    // assert
    TEST_ASSERT(ok);
    TEST_ASSERT(args.lcdType == LCD_NONE);
}

void test_lcdTypeIs1602WhenSet(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {"--rom", EXISTING_ROM_FILE, "--lcd", "1602"};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 4, argv, errorBuffer);

    // assert
    TEST_ASSERT(ok);
    TEST_ASSERT(args.lcdType == LCD_1602);
}

void test_lcdTypeIs2004WhenSet(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {"--rom", EXISTING_ROM_FILE, "--lcd", "2004"};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 4, argv, errorBuffer);

    // assert
    TEST_ASSERT(ok);
    TEST_ASSERT(args.lcdType == LCD_2004);
}

void test_lcdTypeIsGraphicsWhenSet(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {"--rom", EXISTING_ROM_FILE, "--lcd", "12864"};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 4, argv, errorBuffer);

    // assert
    TEST_ASSERT(ok);
    TEST_ASSERT(args.lcdType == LCD_GRAPHICS);
}

void test_errorIfLcdTypeIsUnknown(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {"--rom", EXISTING_ROM_FILE, "--lcd", "1111111"};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 4, argv, errorBuffer);

    // assert
    TEST_ASSERT(!ok);
    TEST_ASSERT(strcmp(errorBuffer, "Unknown LCD type: 1111111") == 0);
}

void test_errorOnLcdTypeOptionAtTheEnd(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {"--rom", EXISTING_ROM_FILE, "--lcd"};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 3, argv, errorBuffer);

    // assert
    TEST_ASSERT(!ok);
    TEST_ASSERT(strcmp(errorBuffer, "No LCD type set") == 0);
}

void test_parsingCfCardFileNoErrorOnExistingFile(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* cfCardImageFile = EXISTING_CFCARD_FILE;
    char* argv[] = {"--rom", EXISTING_ROM_FILE, "--cfcard", cfCardImageFile};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 4, argv, errorBuffer);

    // assert
    TEST_ASSERT(ok);
    TEST_ASSERT(strcmp(args.cfCardImageFile, cfCardImageFile) == 0);
}

void test_parsingCfCardFileErrorOnNonExistingFile(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* cfCardImageFile = "test/test_parseargs_data/non_existing_card_image.bin";
    char* argv[] = {"--rom", EXISTING_ROM_FILE, "--cfcard", cfCardImageFile};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 4, argv, errorBuffer);

    // assert
    TEST_ASSERT(!ok);
    TEST_ASSERT(args.cfCardImageFile == NULL);
    TEST_ASSERT(strcmp(errorBuffer, "File 'test/test_parseargs_data/non_existing_card_image.bin' cannot be found.") == 0);
}

void test_errorOnCfCardOptionAtTheEnd(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {"--cfcard"};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 1, argv, errorBuffer);

    // assert
    TEST_ASSERT(!ok);
    TEST_ASSERT(strcmp(errorBuffer, "No CompactFlash card image file specified. Use --cfcard <filename> to set it.") == 0);
}

void test_parsingMissingExitLabel(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {"--rom", EXISTING_ROM_FILE, "--exit-label"};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 3, argv, errorBuffer);

    // assert
    TEST_CHECK(!ok);
    TEST_CHECK(args.exitLabel == NULL);
    TEST_CHECK(strcmp(errorBuffer, "Exit label not provided. Use --exit-label <label> to set it.") == 0);
}

void test_parsingExistingExitLabel(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* expectedExitLabel = "this_is_my_label";
    char* argv[] = {"--rom", EXISTING_ROM_FILE, "--exit-label", expectedExitLabel};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 4, argv, errorBuffer);

    // assert
    TEST_CHECK(ok);
    TEST_CHECK(strcmp(args.exitLabel, expectedExitLabel) == 0);
}

void test_parsingMissingDumpFile(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {"--rom", EXISTING_ROM_FILE, "--dump-memory-on-exit"};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 3, argv, errorBuffer);

    // assert
    TEST_CHECK(!ok);
    TEST_CHECK(args.dumpMemoryFile == NULL);
    TEST_CHECK(strcmp(errorBuffer, "Missing memory dump file path. Use --dump-memory-on-exit <filename> to set it.") == 0);
}

void test_parsingExistingDumpFile(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* expectedDumpFile = "memory.dmp";
    char* argv[] = {"--rom", EXISTING_ROM_FILE, "--dump-memory-on-exit", expectedDumpFile};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 4, argv, errorBuffer);

    // assert
    TEST_CHECK(ok);
    TEST_CHECK(strcmp(args.dumpMemoryFile, expectedDumpFile) == 0);
}

TEST_LIST = {
    { "test_parsingFailsOnMissingArgv", test_parsingFailsOnMissingArgv },
    { "test_parsingRomFileNoErrorOnMissingRomOption", test_parsingRomFileNoErrorOnMissingRomOption },
    { "test_parsingRomFileNoErrorOnExistingFile", test_parsingRomFileNoErrorOnExistingFile },
    { "test_parsingRomFileErrorOnNonExistingFile", test_parsingRomFileErrorOnNonExistingFile },
    { "test_parsingRomFileErrorOnIncorrectFileSize", test_parsingRomFileErrorOnIncorrectFileSize },
    { "test_errorOnRomOptionAtTheEnd", test_errorOnRomOptionAtTheEnd },
    { "test_breakOnStartIsFalseByDefault", test_breakOnStartIsFalseByDefault },
    { "test_breakOnStartIsTrueWhenSet", test_breakOnStartIsTrueWhenSet },
    { "test_lcdTypeIs1602WhenSet", test_lcdTypeIs1602WhenSet },
    { "test_lcdTypeIs2004WhenSet", test_lcdTypeIs2004WhenSet },
    { "test_lcdTypeIsGraphicsWhenSet", test_lcdTypeIsGraphicsWhenSet },
    { "test_errorIfLcdTypeIsUnknown", test_errorIfLcdTypeIsUnknown },
    { "test_errorOnLcdTypeOptionAtTheEnd", test_errorOnLcdTypeOptionAtTheEnd },
    { "test_parsingCfCardFileNoErrorOnExistingFile", test_parsingCfCardFileNoErrorOnExistingFile },
    { "test_parsingCfCardFileErrorOnNonExistingFile", test_parsingCfCardFileErrorOnNonExistingFile },
    { "test_parsingMissingExitLabel", test_parsingMissingExitLabel },
    { "test_parsingExistingExitLabel", test_parsingExistingExitLabel },
    { "test_parsingMissingDumpFile", test_parsingMissingDumpFile },
    { "test_parsingExistingDumpFile", test_parsingExistingDumpFile },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};