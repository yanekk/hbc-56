#include "acutest.h"
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stddef.h>

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

#define TEST_FILE_SIZE 1024
void test_fileNew(void) {
    // act
    File* fileInfo = File_New(TEST_FILE_SIZE);
    fileInfo->data[0] = 'a';
    fileInfo->data[TEST_FILE_SIZE-1] = 'z';

    // assert
    TEST_CHECK_(fileInfo->size == TEST_FILE_SIZE, "expected file size to be %d, got %lld", 1024, fileInfo->size);
    TEST_CHECK_(fileInfo->isOk, "expected file ok state to be true, got false");
    TEST_CHECK_(fileInfo->data[0] == 'a', "expected char to be \"%c\", but got \"%c\"", 'a', fileInfo->data[0]);
    TEST_CHECK_(fileInfo->data[TEST_FILE_SIZE-1] == 'z', "expected char to be \"%c\", but got \"%c\"", 'z', fileInfo->data[TEST_FILE_SIZE-1]);

    //cleanup
    File_Free(fileInfo);
}

void test_fileSave() {
    // arrange
    srand(time(NULL));
    char fileName[128];
    sprintf(fileName, "test/test_file_data/new_file-%d.bin", rand());

    File* newFile = File_New(TEST_FILE_SIZE);
    newFile->data[0] = 'a';
    newFile->data[TEST_FILE_SIZE-1] = 'z';

    // act
    bool isFileSaved = File_Save(fileName, newFile);

    // assert
    TEST_CHECK_(isFileSaved, "expected File_Save to return true, got false");
    
    File* savedFile = File_Read(fileName);
    TEST_CHECK_(savedFile->size == TEST_FILE_SIZE, "expected file size to be %d, got %lld", TEST_FILE_SIZE, savedFile->size);
    TEST_CHECK_(savedFile->isOk, "expected file ok state to be true, got false");
    TEST_CHECK_(savedFile->data[0] == 'a', "expected char to be \"%c\", but got \"%c\"", 'a', savedFile->data[0]);
    TEST_CHECK_(savedFile->data[TEST_FILE_SIZE-1] == 'z', "expected char to be \"%c\", but got \"%c\"", 'z', savedFile->data[TEST_FILE_SIZE-1]);

    // clean
    File_Free(newFile);
    File_Delete(fileName);
    File_Free(savedFile);
}

void test_fileDelete() {
    // arrange
    char fileName[128];
    sprintf(fileName, "test/test_file_data/new_file-%d.bin", rand());

    File* newFile = File_New(1024);
    File_Save(fileName, newFile);
    File_Free(newFile);

    // act
    File_Delete(fileName);

    // assert
    File* deletedFile = File_Read(fileName);
    TEST_CHECK_(!deletedFile->isOk, "expected deleted file ok state to be false, got true");

    // clean
    File_Free(deletedFile);
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
    { "test_fileNew", test_fileNew },
    { "test_fileSave", test_fileSave },
    { "test_fileDelete", test_fileDelete },
    { NULL, NULL }     /* zeroed record marking the end of the list */
};
