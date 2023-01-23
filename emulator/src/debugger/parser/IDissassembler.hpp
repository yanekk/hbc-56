#pragma once
#include "stdint.h"

class IDissassembler
{
    public:
        virtual ~IDissassembler() {}
        virtual uint16_t disassemble(uint16_t tempAddress, int bufferSize, char* buffer, uint16_t* refAddr) = 0;
};