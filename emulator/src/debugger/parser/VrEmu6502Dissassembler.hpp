#pragma once
#include "IDissassembler.hpp"
#include "vrEmu6502.h"
#include "debugger/label_file.h"

class VrEmu6502Dissassembler : public IDissassembler
{
    public:
        VrEmu6502Dissassembler(VrEmu6502* cpu6502, char** labelMap);
        virtual uint16_t disassemble(uint16_t tempAddress, int bufferSize, char* buffer, uint16_t* refAddr);
    private:
        VrEmu6502* cpu6502;
        char** labelMap;
};