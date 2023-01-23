#include "VrEmu6502Dissassembler.hpp"
#include "stdint.h"
#include "vrEmu6502.h"

VrEmu6502Dissassembler::VrEmu6502Dissassembler(VrEmu6502* cpu6502, char** labelMap) 
    : cpu6502(cpu6502), labelMap(labelMap) 
{

}

uint16_t VrEmu6502Dissassembler::disassemble(uint16_t tempAddress, int bufferSize, char* buffer, uint16_t* refAddr) {
    return vrEmu6502DisassembleInstruction(cpu6502, tempAddress, sizeof(bufferSize), buffer, refAddr, labelMap);
}