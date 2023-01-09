#include "acutest.h"
#include <stdbool.h>
#include <string.h>

#include "file.h"

#define EXISTING_FILE "test/test_file_data/existing_file.bin"
#define NON_EXISTING_FILE "test/test_file_data/non_existing_file.bin"

void test_nonExistingFileIsNotOk(void)
{
    // act
    File* file = File_Read(NON_EXISTING_FILE);

    // assert
    TEST_ASSERT(!file->isOk);
}

void test_nonExistingFileHasNoData(void)
{
    // act
    File* file = File_Read(NON_EXISTING_FILE);

    // assert
    TEST_ASSERT(file->data == NULL);
}

void test_nonExistingFileHasNoSize(void)
{
    // act
    File* file = File_Read(NON_EXISTING_FILE);

    // assert
    TEST_ASSERT(file->size == 0);
}

void test_existingFileIsOk(void)
{
    // act
    File* fileInfo = File_Read(EXISTING_FILE);

    // assert
    TEST_ASSERT(fileInfo->isOk);
}

void test_existingFileHasCorrectSize(void)
{
    // act
    File* fileInfo = File_Read(EXISTING_FILE);

    // assert
    TEST_ASSERT(fileInfo->size == 1000);
}

void test_existingFileHasPointerToData(void)
{
    // act
    File* fileInfo = File_Read(EXISTING_FILE);

    // assert
    TEST_ASSERT(fileInfo->data != NULL);
    TEST_ASSERT(strcmp((char*)fileInfo->data, "start") == 0);
    TEST_ASSERT(strcmp((char*)fileInfo->data+1000-4, "end") == 0);
}

void test_existingFileIsReadable(void)
{
    // act
    File* fileInfo = File_ReadWithSuffix(EXISTING_FILE, ".suffix");

    // assert
    TEST_ASSERT(fileInfo->isOk);
    TEST_ASSERT(fileInfo->size == 1000);

    TEST_ASSERT(fileInfo->data != NULL);
    TEST_ASSERT(strcmp((char*)fileInfo->data, "suffix_start") == 0);
    TEST_ASSERT(strcmp((char*)fileInfo->data+1000-11, "suffix_end") == 0);
}

void test_freeFileReturnsOk(void)
{
    // arrange
    File* fileInfo = File_Read(EXISTING_FILE);

    // act
    bool ok = File_Free(fileInfo);

    // assert
    TEST_ASSERT(ok);
}

TEST_LIST = {
    { "test_nonExistingFileIsNotOk", test_nonExistingFileIsNotOk },
    { "test_nonExistingFileHasNoData", test_nonExistingFileHasNoData },
    { "test_nonExistingFileHasNoSize", test_nonExistingFileHasNoSize },
    { "test_existingFileIsOk", test_existingFileIsOk },
    { "test_existingFileHasCorrectSize", test_existingFileHasCorrectSize },
    { "test_existingFileHasPointerToData", test_existingFileHasPointerToData },
    { "test_existingFileIsReadable", test_existingFileIsReadable },
    { "test_freeFileReturnsOk", test_freeFileReturnsOk },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};