#include "acutest.h"
#include <stdbool.h>
#include <string.h>

#include "debugger/parser/Token.hpp"

void test_tokenHasTypeAndValue()
{
    // arrange
    Token::Ptr tokenPtr = Token::Create(Token::LABEL, "label1");

    // assert
    TEST_CHECK(tokenPtr->type() == Token::LABEL);
    TEST_CHECK(tokenPtr->value() == "label1");
}

TEST_LIST = {
    { "test_tokenHasTypeAndValue", test_tokenHasTypeAndValue },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};