#pragma once

#include <string>
#include "Source.hpp"
#include "SourceFile.hpp"

class Source
{
public:
  Source(std::set<std::string>& opcodes, IDissassembler& dissassembler);
  SourceFile& file(const std::string& filename);

  const SourceFile& file(uint16_t addr);

  const SourceFile& fileFromIndex(size_t index) const;

  int numFiles() const;
  int index(const std::string& filename);

  void clear();

  private:
    std::map<uint16_t, size_t> m_addrMap;
    std::map<std::string, SourceFile> m_files;
    std::set<std::string>& m_opcodes;
    IDissassembler& m_dissassembler;
};