#include "acutest.h"
#include <stdbool.h>
#include <string.h>

#include <set>

#include "debugger/parser/SourceLine.hpp"

void test_labelIsParsedIfNoOpcodeDefined()
{
    // arrange
    std::set<std::string> opcodes;

    // act
    SourceLine sourceLine("ADC", opcodes);

    // assert
    Token* token = sourceLine.children()[0].get();
    TEST_CHECK(token->type() == Token::LABEL);
    TEST_CHECK(token->value() == "ADC");
}

void test_opcodeIsParsedIfOpcodeDefined()
{
    // arrange
    std::set<std::string> opcodes;
    opcodes.insert("ADC");

    // act
    SourceLine sourceLine("ADC", opcodes);

    // assert
    Token* token = sourceLine.children()[0].get();
    TEST_CHECK(token->type() == Token::OPCODE);
    TEST_CHECK(token->value() == "ADC");
}

void test_macro()
{
    // arrange
    std::set<std::string> opcodes;

    // act
    SourceLine sourceLine("+macro_name", opcodes);

    // assert
    Token* token = sourceLine.children()[0].get();
    TEST_CHECK(token->type() == Token::MACRO);
    TEST_CHECK_(token->value() == "+macro_name", "got macro token value \"%s\" but expected \"%s\"", token->value().c_str(), "macro_name");
}

TEST_LIST = {
    { "test_labelIsParsedIfNoOpcodeDefined", test_labelIsParsedIfNoOpcodeDefined },
    { "test_opcodeIsParsedIfOpcodeDefined", test_opcodeIsParsedIfOpcodeDefined },
    { "test_macro", test_macro },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};