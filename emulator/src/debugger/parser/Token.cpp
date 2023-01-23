#include "Token.hpp"

Token::Ptr Token::Create(Token::Type type, const std::string& value) {
    return Token::Ptr(new Token(type, value));
}

Token::Type Token::type() const {
    return m_type;
}

const std::string& Token::value() const {
    return m_value;
}

Token::Token(Token::Type type, const std::string& value)
    : m_type(type), m_value(value)
{

}