#include "SourceFile.hpp"
#include "IDissassembler.hpp"

SourceFile::SourceFile() {

}

SourceFile::SourceFile(std::set<std::string>& opcodes, IDissassembler& dissassembler) 
    : m_opcodes(&opcodes), m_dissassembler(&dissassembler) 
{

}

SourceFile::SourceFile(const std::string& filename, std::set<std::string>& opcodes, IDissassembler& dissassembler) 
    : m_filename(filename), m_opcodes(&opcodes), m_dissassembler(&dissassembler) 
{

}

void SourceFile::addLine(const std::string& line, uint16_t addr) {
    m_addrMap[addr] = (int)m_lines.size();
    m_lines.push_back(SourceLine(line, *m_opcodes, addr));

    if (addr == 0xffff)
        return;
    

    for (int i = (int)m_lines.size() - 2; i >= 0; --i) {
        if (m_lines[i].address() == 0xffff) {
            m_lines[i].setAddress(addr);

            if (m_lines[i].contains(Token::LABEL) && m_constants.find(m_lines[i].childOfType(Token::LABEL)->value()) == m_constants.end()) {
                if (!m_lines[i].contains(Token::OPERATOR))
                    m_constants[m_lines[i].childOfType(Token::LABEL)->value()] = addr;
            }
        } else {
            if (m_lines[i].contains(Token::MACRO))
            {
                uint16_t fromAddress = m_lines[i].address();
                uint16_t toAddress = addr;

                uint16_t tmpAddress = fromAddress;
                char instructionBuffer[32];
                while (tmpAddress < toAddress) {
                    uint16_t refAddr = 0;
                    uint16_t prevTmpAddress = tmpAddress;
                    tmpAddress = m_dissassembler->disassemble(tmpAddress, sizeof(instructionBuffer), instructionBuffer, &refAddr);
                    m_lines[i].macroLines().push_back(SourceLine(instructionBuffer, *m_opcodes, prevTmpAddress));
                }
            }
            break;
        }
    }
}

const std::string& SourceFile::filename() const { 
    return m_filename; 
}

int SourceFile::numLines() const { 
    return (int)m_lines.size(); 
}

const SourceLine& SourceFile::line(size_t index) const {
    return m_lines[index];
}

int SourceFile::lineIndex(uint16_t addr) const {
    auto iter = m_addrMap.lower_bound(addr);
    if (iter == m_addrMap.end()) 
        return -1;

    if (iter->first == addr)
        return iter->second;
    
    if (iter == m_addrMap.begin())
        return -1;

    --iter;

    if (addr - iter->first < 50)
        return iter->second;

    return -1;
}