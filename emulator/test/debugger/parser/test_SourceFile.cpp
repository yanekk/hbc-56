#include "acutest.h"
#include <stdbool.h>
#include <string.h>

#include <set>

#include "debugger/parser/SourceFile.hpp"
#include "debugger/parser/IDissassembler.hpp"

class StubDisassembler : public IDissassembler {
    uint16_t disassemble(uint16_t tempAddress, int bufferSize, char* buffer, uint16_t* refAddr) {
        printf("StubDisassembler::disassemble");
        return 0;
    }
};

void test_test()
{   // arrange
    std::set<std::string> opcodes;
    StubDisassembler disassembler;
    SourceFile sourceFile(opcodes, disassembler);

    // act
    sourceFile.addLine("+macro_name", 0x1234);
}

TEST_LIST = {
    { "test_test", test_test },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};