#include "acutest.h"
#include <stdbool.h>
#include <string.h>

#include "file.h"
#include "debugger/label_file.h"

#define DATA_PATH "test/debugger/test_label_file_data/"
#define TEST_CHECK_LABEL_EQUALS(index, expected) \
    TEST_CHECK_(strcmp(labelMap[index], expected) == 0, "expected label at %x to be '%s', found '%s'", index, expected, labelMap[index])
#define TEST_CHECK_LABEL_IS_NULL(index) \ 
    TEST_CHECK_(labelMap[index] == NULL, "expected label at %d to be NULL, found '%s'", index, labelMap[index])

char* labelMap[0x10000];
void test_parsesLabelFileToArray(void)
{
    // arrange
    File* file = File_Read(DATA_PATH "simple_label_file.lmap");

    // act
    Debugger_LoadLabels(file->data, labelMap);

    // assert
    TEST_CHECK_LABEL_EQUALS(0x0, "label_1");
    TEST_CHECK_LABEL_EQUALS(0x10, "label_2");
    TEST_CHECK_LABEL_EQUALS(0x100, "label_3");
    TEST_CHECK_LABEL_EQUALS(0x1000, "label_4");

    // cleanup
    File_Free(file);
}

void test_cleansUpLabelsOnInvocation(void)
{
    // arrange
    labelMap[0] = malloc(sizeof(char) * 64); strcpy(labelMap[0], "label");
    labelMap[1] = malloc(sizeof(char) * 64); strcpy(labelMap[1], "other_label");
    labelMap[2] = malloc(sizeof(char) * 64); strcpy(labelMap[2], "yet_another_label");
    labelMap[0x10000-1] = malloc(sizeof(char) * 64); strcpy(labelMap[0x10000-1], "last_label"); 

    // act
    
    Debugger_LoadLabels(NULL, labelMap);

    // assert
    TEST_CHECK_LABEL_IS_NULL(0);
    TEST_CHECK_LABEL_IS_NULL(1);
    TEST_CHECK_LABEL_IS_NULL(2);
    TEST_CHECK_LABEL_IS_NULL(0x10000-1);
}

void test_nonConstantLabelsDoesntOverrideAddresses(void)
{
    // arrange
    File* file = File_Read(DATA_PATH "label_file_with_no_constant_override.lmap");

    // act
    Debugger_LoadLabels(file->data, labelMap);

    // assert
    TEST_CHECK_LABEL_EQUALS(0x1234, "label_one");

    // cleanup
    File_Free(file);
}

void test_unusedConstantLabelsDoesntOverrideAddresses(void)
{
    // arrange
    File* file = File_Read(DATA_PATH "label_file_with_unused_constant_override.lmap");

    // act
    Debugger_LoadLabels(file->data, labelMap);

    // assert
    TEST_CHECK_LABEL_EQUALS(0x1234, "LABEL_ONE");

    // cleanup
    File_Free(file);
}

void test_usedConstantLabelsOverrideAddresses(void)
{
    // arrange
    File* file = File_Read(DATA_PATH "label_file_with_constant_override.lmap");

    // act
    Debugger_LoadLabels(file->data, labelMap);

    // assert
    TEST_CHECK_LABEL_EQUALS(0x1234, "label_two");

    // cleanup
    File_Free(file);
}

TEST_LIST = {
    { "test_parsesLabelFileToArray", test_parsesLabelFileToArray },
    { "test_cleansUpLabelsOnInvocation", test_cleansUpLabelsOnInvocation },
    { "test_nonConstantLabelsDoesntOverrideAddresses", test_nonConstantLabelsDoesntOverrideAddresses },
    { "test_unusedConstantLabelsDoesntOverrideAddresses", test_unusedConstantLabelsDoesntOverrideAddresses },
    { "test_usedConstantLabelsOverrideAddresses", test_usedConstantLabelsOverrideAddresses },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};