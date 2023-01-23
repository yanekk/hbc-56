#pragma once

#include <string>
#include <memory>

class Token
{
public:
  typedef std::shared_ptr<Token> Ptr;

  typedef enum
  {
    WHITESPACE,
    COMMENT,
    OPCODE,
    COMMA,
    NUMBER,
    STRING,
    OPERATOR,
    IMMEDIATE,
    MACRO,
    CONSTANT,
    LABEL,
    PSEUDOOP,
    UNKNOWN
  } Type;

  static Ptr Create(Token::Type type, const std::string& value);

  Token::Type type() const;
  const std::string& value() const;

private:
  Token(Token::Type type, const std::string& value);

  Token::Type             m_type;
  std::string             m_value;
};
