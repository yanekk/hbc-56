#include "acutest.h"
#include "parseargs.h"

#include <stdbool.h>
#include <string.h>

void test_parsingFailsOnMissingArgv(void)
{
    // act
    Hbc56EmulatorArgs* args = Hbc56EmulatorArgs_Parse(0, NULL);

    // assert
    TEST_ASSERT(args == NULL);
}

void test_parsingSucceededOnPassedArgs(void)
{
    // arrange
    char* argv[] = {"--option-one", "option_value"};

    // act
    Hbc56EmulatorArgs* args = Hbc56EmulatorArgs_Parse(2, argv);

    // assert
    TEST_ASSERT(args != NULL);
}

void test_parsingRomFile(void)
{
    // arrange
    char* romFile = "path/to/rom.bin";
    char* argv[] = {"--rom", romFile};

    // act
    Hbc56EmulatorArgs* args  = Hbc56EmulatorArgs_Parse(2, argv);

    // assert
    TEST_ASSERT(args != NULL);

    TEST_ASSERT(strcmp(args->romFile, romFile) == 0);
}
// TODO: destroy arg pointer
TEST_LIST = {
   { "test_parsingFailsOnMissingArgv", test_parsingFailsOnMissingArgv },
   { "test_parsingSucceededOnPassedArgs", test_parsingSucceededOnPassedArgs },
   { "test_parsingRomFile", test_parsingRomFile },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};