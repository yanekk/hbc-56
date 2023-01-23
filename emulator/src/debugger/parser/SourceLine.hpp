#pragma once

#include <string>
#include <vector>
#include <set>
#include "Token.hpp"

class SourceLine
{
public:
  //SourceLine(SourceFile *file, const std::string& line, uint16_t addr = 0xffff);
  SourceLine(const std::string& line, std::set<std::string>& opcodes, uint16_t addr = 0xffff);

  void setAddress(uint16_t addr);

  //SourceFile& file() const;
  uint16_t address() const;
  const std::vector<Token::Ptr> &children() const;
  std::vector<SourceLine>& macroLines();
  const std::vector<SourceLine>& macroLines() const;
  bool contains(Token::Type type) const;
  Token::Ptr childOfType(Token::Type type) const;
  bool containsOnly(Token::Type type) const;

private:
  void addChild(Token::Ptr child);

private:
  //SourceFile             *m_file;
  uint16_t                m_addr;
  std::vector<Token::Ptr> m_children;
  std::vector<SourceLine> m_macroLines;
};
