#include "acutest.h"
#include "parseargs.h"

#include <stdbool.h>
#include <string.h>

void test_parsingFailsOnMissingArgv(void)
{
    // arrange
    const Hbc56EmulatorArgs args = {};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(0, NULL, &args);

    // assert
    TEST_ASSERT(ok == false);
}

void test_parsingSucceededOnPassedArgs(void)
{
    // arrange
    const Hbc56EmulatorArgs args = {};
    char* argv[] = {"arg1", "arg2"};

    // act
    bool ok = Hbc56EmulatorArgs_Parse(0, argv, &args);

    // assert
    TEST_ASSERT(ok == true);
}

TEST_LIST = {
   { "test_parsingFailsOnMissingArgv", test_parsingFailsOnMissingArgv },
   { "test_parsingSucceededOnPassedArgs", test_parsingSucceededOnPassedArgs },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};