#include "Source.hpp"


Source::Source(std::set<std::string>& opcodes, IDissassembler& dissassembler) 
    : m_opcodes(opcodes), m_dissassembler(dissassembler) 
{

}

  SourceFile& Source::file(const std::string& filename)
  {
    auto iter = m_files.find(filename);
    if (iter == m_files.end())
    {
      m_files[filename] = SourceFile(filename, m_opcodes, m_dissassembler);
      return m_files[filename];
    }
    return iter->second;
  }

  const SourceFile& Source::file(uint16_t addr)
  {
    if (m_addrMap.empty())
    {
      int fileIndex = 0;
      for (auto iter = m_files.begin(); iter != m_files.end(); ++iter, ++fileIndex)
      {
        for (int i = 0; i < iter->second.numLines(); ++i)
        {
          m_addrMap[iter->second.line(i).address()] = fileIndex;
        }
      }
    }

    static SourceFile nullFile("", m_opcodes, m_dissassembler);

    auto iter = m_addrMap.lower_bound(addr);
    if (iter == m_addrMap.end()) return nullFile;

    return fileFromIndex(iter->second);
  }

  const SourceFile& Source::fileFromIndex(size_t index) const {
    auto iter = m_files.begin();
    std::advance(iter, index);
    return iter->second;
  }

  int Source::numFiles() const { return (int)m_files.size(); }
  int Source::index(const std::string& filename)
  {
    int i = 0;
    for (auto iter = m_files.begin(); iter != m_files.end(); ++iter, ++i)
    {
      if (iter->first == filename)
        return i;
    }
    return -1;
  }

  void Source::clear()
  {
    m_addrMap.clear();
    m_files.clear();
  }