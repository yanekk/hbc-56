#pragma once
#include <string>
#include <stdint.h>
#include <map>
#include <vector>

#include "SourceLine.hpp"
#include "IDissassembler.hpp"

class SourceFile
{
  public:
    SourceFile();
    SourceFile(std::set<std::string>& opcodes, IDissassembler& dissassembler);
    SourceFile(const std::string& filename, std::set<std::string>& opcodes, IDissassembler& dissassembler);

    void addLine(const std::string& line, uint16_t addr);

    const std::string &filename() const;

    int numLines() const;
    const SourceLine& line(size_t index) const;

    int lineIndex(uint16_t addr) const;

  private:
    std::string m_filename;
    std::vector<SourceLine> m_lines;
    std::map<uint16_t, int> m_addrMap;
    std::set<std::string>* m_opcodes;
    std::map<std::string, int> m_constants;
    IDissassembler* m_dissassembler;
};