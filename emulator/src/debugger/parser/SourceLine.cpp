#include <string>
#include <vector>
#include <set>
#include <iostream>

#include "Token.hpp"
#include "SourceLine.hpp"

static std::set<char> operators = {'!','^','-','/','%','+','<','>','=','&','|','(',')'};

SourceLine::SourceLine(const std::string& line, std::set<std::string>& opcodes, uint16_t addr) : m_addr(addr)
{
  for (std::size_t i = 0; i < line.size(); ++i)
  {
    if (line[i] == ';')
    {
      addChild(Token::Create(Token::COMMENT, line.substr(i)));
      return;
    }
    else if (line[i] == '\'')
    {
      size_t start = i;
      ++i;
      for (; i < line.size(); ++i)
      {
        if (line[i] == '\'' && line[i - 1] != '\\') break;
      }
      addChild(Token::Create(Token::STRING, line.substr(start, i - start)));
      --i;
    }
    else if (line[i] == '"')
    {
      size_t start = i;
      ++i;
      for (; i < line.size(); ++i)
      {
        if (line[i] == '"' && line[i - 1] != '\\') break;
      }
      addChild(Token::Create(Token::STRING, line.substr(start, i - start)));
      --i;
    }
    else if (isspace(line[i]))
    {
      size_t start = i;
      for (; i < line.size(); ++i)
      {
        if (!isspace(line[i])) break;
      }
      addChild(Token::Create(Token::WHITESPACE, line.substr(start, i - start)));
      --i;
    }
    else if (line[i] == ',')
    {
      addChild(Token::Create(Token::COMMA, line.substr(i, 1)));
    }
    else if (line[i] == '#')
    {
      addChild(Token::Create(Token::IMMEDIATE, line.substr(i, 1)));
    }
    else if (line[i] == '+' && (containsOnly(Token::WHITESPACE)))
    {
      size_t start = i++;
      for (; i < line.size(); ++i)
      {
        if (!isalnum(line[i]) && line[i] != '_' && line[i] <= 127) break;
      }
      addChild(Token::Create(i - start == 1 ? Token::CONSTANT : Token::MACRO, line.substr(start, i - start)));
      --i;
    }
    else if (line[i] == '!' && (containsOnly(Token::WHITESPACE)))
    {
      size_t start = i++;
      for (; i < line.size(); ++i)
      {
        if (!isalpha(line[i])) break;
      }
      addChild(Token::Create(Token::PSEUDOOP, line.substr(start, i - start)));
      --i;
    }
    else if (line[i] == '.' || line[i] == '@' || line[i] == '_' || isalpha(line[i]))
    {
      size_t start = i++;
      for (; i < line.size(); ++i)
      {
        if (!isalnum(line[i]) && line[i] != '_' && line[i] <= 127) break;
      }

      Token::Type type = start == 0 ? Token::LABEL : Token::CONSTANT;
      std::string word = line.substr(start, i - start);

      if (isalpha(line[start]))
      {
        if (opcodes.find(word) != opcodes.end())
        {
          type = Token::OPCODE;
        }
        else if (word == "DIV")
        {
          type = Token::OPERATOR;
        }
      }

      addChild(Token::Create(type, word));
      --i;
    }
    else if (line[i] == '$' || (line[i] == '0' && line[i + 1] == 'x')) // hex
    {
      size_t start = i++;
      for (; i < line.size(); ++i)
      {
        if (!isxdigit(line[i])) break;
      }
      addChild(Token::Create(Token::NUMBER, line.substr(start, i - start)));
      --i;
    }
    else if (line[i] == '%' && (line[i + 1] == '0' || line[i + 1] == '1' || line[i + 1] == '#' || line[i + 1] == '.')) // binary
    {
      size_t start = i++;
      for (; i < line.size(); ++i)
      {
        if (line[i] != '0' && line[i] != '1' && line[i] != '.' && line[i] != '#') break;
      }
      addChild(Token::Create(Token::NUMBER, line.substr(start, i - start)));
      --i;
    }
    else if (line[i] == '&' && isdigit(line[i + 1])) // octal
    {
      size_t start = i++;
      for (; i < line.size(); ++i)
      {
        if (line[i] < '0' || line[i]  > '7') break;
      }
      addChild(Token::Create(Token::NUMBER, line.substr(start, i - start)));
      --i;
    }
    else if (isdigit(line[i]))
    {
      size_t start = i;
      for (; i < line.size(); ++i)
      {
        if (!isdigit(line[i]) && line[i] != '.') break;
      }
      addChild(Token::Create(Token::NUMBER, line.substr(start, i - start)));
      --i;
    }
    else
    {
      size_t start = i;
      if (operators.find(line[i]) != operators.end())
      {
        for (; i < line.size(); ++i)
        {
          if (operators.find(line[i]) == operators.end()) break;
        }
        addChild(Token::Create(Token::OPERATOR, line.substr(start, i - start)));
        --i;
      }
      else
      {
        addChild(Token::Create(Token::UNKNOWN, line.substr(i, 1)));
      }
    }
  }
}

void SourceLine::setAddress(uint16_t addr) { 
  m_addr = addr; 
}

uint16_t SourceLine::address() const { 
  return m_addr; 
}

const std::vector<Token::Ptr>& SourceLine::children() const { 
  return m_children; 
}

std::vector<SourceLine>& SourceLine::macroLines() { 
  return m_macroLines; 
}

const std::vector<SourceLine>& SourceLine::macroLines() const { 
  return m_macroLines; 
}

bool SourceLine::contains(Token::Type type) const
{
  for (const auto &node : m_children)
  {
    if (node->type() == type)
      return true;
  }
  return false;
}

Token::Ptr SourceLine::childOfType(Token::Type type) const
{
    for (const auto& node : m_children)
    {
        if (node->type() == type)
            return node;
    }
    return nullptr;
}

bool SourceLine::containsOnly(Token::Type type) const
{
    for (const auto& node : m_children)
    {
        if (node->type() != type)
            return false;
    }
    return true;
}

void SourceLine::addChild(Token::Ptr child) { 
    m_children.push_back(child);     
}

