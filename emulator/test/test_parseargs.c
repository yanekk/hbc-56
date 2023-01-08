#include "acutest.h"
#include "parseargs.h"

#include <stdbool.h>
#include <string.h>

void test_parsingFailsOnMissingArgv(void)
{
    // act
    Hbc56EmulatorArgs args = {0};
    bool ok = Hbc56EmulatorArgs_Parse(&args, 0, NULL);

    // assert
    TEST_ASSERT(!ok);
}

void test_parsingSucceededOnPassedArgs(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {"--option-one", "option_value"};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 2, argv);

    // assert
    TEST_ASSERT(ok);
}

void test_parsingRomFile(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* romFile = "path/to/rom.bin";
    char* argv[] = {"--rom", romFile};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 2, argv);

    // assert
    TEST_ASSERT(ok);
    TEST_ASSERT(strcmp(args.romFile, romFile) == 0);
}

void test_errorOnRomOptionAtTheEnd(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {"--rom"};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 1, argv);

    // assert
    TEST_ASSERT(!ok);
}

void test_breakOnStartIsFalseByDefault(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {"--option-1"};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 1, argv);

    // assert
    TEST_ASSERT(ok);
    TEST_ASSERT(args.breakOnStart == false);
}

void test_breakOnStartIsTrueWhenSet(void)
{
    // arrange
    Hbc56EmulatorArgs args = {0};
    char* argv[] = {"--brk"};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(&args, 1, argv);

    // assert
    TEST_ASSERT(ok);
    TEST_ASSERT(args.breakOnStart == true);
}

TEST_LIST = {
   { "test_parsingFailsOnMissingArgv", test_parsingFailsOnMissingArgv },
   { "test_parsingSucceededOnPassedArgs", test_parsingSucceededOnPassedArgs },
   { "test_parsingRomFile", test_parsingRomFile },
   { "test_errorOnRomOptionAtTheEnd", test_errorOnRomOptionAtTheEnd },
   { "test_breakOnStartIsFalseByDefault", test_breakOnStartIsFalseByDefault },
   { "test_breakOnStartIsTrueWhenSet", test_breakOnStartIsTrueWhenSet },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};