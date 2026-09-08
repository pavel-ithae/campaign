#pragma once

#include "test_api_util.h"

#include <campaign/api/api_layout.h>
#include <campaign/api/api_string.h>

#include <string.h>

#define INIT_CAMPAIGN_LAYOUT_TEST(layoutDescriptorCount) \
    {                                                    \
        void *layoutPtr;                                 \
        int32_t sizeBuffer;                              \
        uint8_t existsBuffer;                            \
        campaign_flag_info_t flagInfoBuffer;             \
        campaign_byte_info_t byteInfoBuffer;             \
        campaign_dynamic_info_t dynamicInfoBuffer;       \
        campaign_layout_entry_info_t entryInfoBuffer;    \
        TEST_ASSERT_CAMPAIGN_API(campaign_layout_create(layoutDescriptorCount, &layoutPtr));

#define CLEAR_CAMPAIGN_LAYOUT_TEST()                             \
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_delete(layoutPtr)); \
    }

#define TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(expectedSize)                          \
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_data_size(layoutPtr, &sizeBuffer)); \
    TEST_ASSERT_EQUAL_INT32(expectedSize, sizeBuffer)

#define TEST_ASSERT_CAMPAIGN_LAYOUT_FLAG_INFO(id, expectedIndex, expectedFlagMask)           \
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_flag_info(layoutPtr, id, &flagInfoBuffer)); \
    TEST_ASSERT_EQUAL_INT32(expectedIndex, flagInfoBuffer.index);                            \
    TEST_ASSERT_EQUAL_INT32(expectedFlagMask, flagInfoBuffer.flagMask)

#define TEST_ASSERT_CAMPAIGN_LAYOUT_BYTE_INFO(id, expectedIndex)                             \
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_byte_info(layoutPtr, id, &byteInfoBuffer)); \
    TEST_ASSERT_EQUAL_INT32(expectedIndex, byteInfoBuffer.index);

#define TEST_ASSERT_CAMPAIGN_LAYOUT_DYNAMIC_INFO(id, expectedIndex, expectedSize)                  \
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_dynamic_info(layoutPtr, id, &dynamicInfoBuffer)); \
    TEST_ASSERT_EQUAL_INT32(expectedIndex, dynamicInfoBuffer.index);                               \
    TEST_ASSERT_EQUAL_INT32(expectedSize, dynamicInfoBuffer.size)

void test_campaign_layout_create_delete(void)
{
    void *layoutPtr;

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_create(4, &layoutPtr));

    TEST_ASSERT_NOT_NULL(layoutPtr);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_delete(layoutPtr));
}

void test_campaign_layout_flag_push(void)
{
    INIT_CAMPAIGN_LAYOUT_TEST(9);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_create(9, &layoutPtr));

    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(0);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_first"));

    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(1);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_second"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_third"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_fourth"));

    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(1);

    TEST_ASSERT_CAMPAIGN_LAYOUT_FLAG_INFO("flag_first", 0, 0x1 << 0);
    TEST_ASSERT_CAMPAIGN_LAYOUT_FLAG_INFO("flag_second", 0, 0x1 << 1);
    TEST_ASSERT_CAMPAIGN_LAYOUT_FLAG_INFO("flag_third", 0, 0x1 << 2);
    TEST_ASSERT_CAMPAIGN_LAYOUT_FLAG_INFO("flag_fourth", 0, 0x1 << 3);

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_flag_info(layoutPtr, "flag_fifth", &flagInfoBuffer));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_fifth"));

    TEST_ASSERT_CAMPAIGN_LAYOUT_FLAG_INFO("flag_fifth", 0, 0x1 << 4);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_sixth"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_seventh"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_eighth"));

    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(1);

    TEST_ASSERT_CAMPAIGN_LAYOUT_FLAG_INFO("flag_sixth", 0, 0x1 << 5);
    TEST_ASSERT_CAMPAIGN_LAYOUT_FLAG_INFO("flag_seventh", 0, 0x1 << 6);
    TEST_ASSERT_CAMPAIGN_LAYOUT_FLAG_INFO("flag_eighth", 0, 0x1 << 7);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_ninth"));

    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(2);

    TEST_ASSERT_CAMPAIGN_LAYOUT_FLAG_INFO("flag_ninth", 1, 0x1 << 0);

    TEST_ASSERT_CAMPAIGN_LAYOUT_FLAG_INFO("flag_first", 0, 0x1 << 0);

    CLEAR_CAMPAIGN_LAYOUT_TEST();
}

void test_campaign_layout_flag_push_empty(void)
{
    INIT_CAMPAIGN_LAYOUT_TEST(10);

    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(0);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag_empty(layoutPtr));

    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(1);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag_empty(layoutPtr));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag_empty(layoutPtr));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag_empty(layoutPtr));

    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(1);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag_empty(layoutPtr));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag_empty(layoutPtr));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag_empty(layoutPtr));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag_empty(layoutPtr));

    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(1);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag_empty(layoutPtr));

    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(2);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag"));

    TEST_ASSERT_CAMPAIGN_LAYOUT_FLAG_INFO("flag", 1, 0x1 << 1);

    CLEAR_CAMPAIGN_LAYOUT_TEST();
}

void test_campaign_layout_byte_push(void)
{
    INIT_CAMPAIGN_LAYOUT_TEST(4);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte(layoutPtr, "byte_first"));

    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(1);

    TEST_ASSERT_CAMPAIGN_LAYOUT_BYTE_INFO("byte_first", 0);

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_byte_info(layoutPtr, "byte_second", &byteInfoBuffer));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte(layoutPtr, "byte_second"));

    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(2);

    TEST_ASSERT_CAMPAIGN_LAYOUT_BYTE_INFO("byte_second", 1);
    TEST_ASSERT_CAMPAIGN_LAYOUT_BYTE_INFO("byte_first", 0);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte(layoutPtr, "byte_third"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte(layoutPtr, "byte_fourth"));

    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(4);

    TEST_ASSERT_CAMPAIGN_LAYOUT_BYTE_INFO("byte_third", 2);
    TEST_ASSERT_CAMPAIGN_LAYOUT_BYTE_INFO("byte_fourth", 3);

    CLEAR_CAMPAIGN_LAYOUT_TEST();
}

void test_campaign_layout_byte_push_empty(void)
{
    INIT_CAMPAIGN_LAYOUT_TEST(4);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte_empty(layoutPtr));

    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(1);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte_empty(layoutPtr));

    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(2);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte_empty(layoutPtr));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte_empty(layoutPtr));

    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(4);

    CLEAR_CAMPAIGN_LAYOUT_TEST();
}

void test_campaign_layout_dynamic_push(void)
{
    INIT_CAMPAIGN_LAYOUT_TEST(4);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic(layoutPtr, "dynamic_byte", 1));
    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(1);

    TEST_ASSERT_CAMPAIGN_LAYOUT_DYNAMIC_INFO("dynamic_byte", 0, 1);

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_dynamic_info(layoutPtr, "dynamic_int", &dynamicInfoBuffer));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic(layoutPtr, "dynamic_int", 4));
    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(5);

    TEST_ASSERT_CAMPAIGN_LAYOUT_DYNAMIC_INFO("dynamic_int", 1, 4);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic(layoutPtr, "dynamic_char", 1));
    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(6);

    TEST_ASSERT_CAMPAIGN_LAYOUT_DYNAMIC_INFO("dynamic_char", 5, 1);

    CLEAR_CAMPAIGN_LAYOUT_TEST();
}

void test_campaign_layout_dynamic_push_empty(void)
{
    INIT_CAMPAIGN_LAYOUT_TEST(4);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic_empty(layoutPtr, 1));
    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(1);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic_empty(layoutPtr, 4));
    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(5);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic_empty(layoutPtr, 1));
    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(6);

    CLEAR_CAMPAIGN_LAYOUT_TEST();
}

void test_campaign_layout_mixed_push(void)
{
    INIT_CAMPAIGN_LAYOUT_TEST(4);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_first"));
    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(1);

    TEST_ASSERT_CAMPAIGN_LAYOUT_FLAG_INFO("flag_first", 0, 0x1 << 0);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte(layoutPtr, "byte_first"));
    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(2);

    TEST_ASSERT_CAMPAIGN_LAYOUT_BYTE_INFO("byte_first", 1);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_second"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_third"));
    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(3);

    TEST_ASSERT_CAMPAIGN_LAYOUT_FLAG_INFO("flag_second", 2, 0x1 << 0);
    TEST_ASSERT_CAMPAIGN_LAYOUT_FLAG_INFO("flag_third", 2, 0x1 << 1);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic(layoutPtr, "dynamic_int", 4));
    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(7);

    TEST_ASSERT_CAMPAIGN_LAYOUT_DYNAMIC_INFO("dynamic_int", 3, 4);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_fourth"));
    TEST_ASSERT_CAMPAIGN_LAYOUT_DATA_SIZE(8);

    TEST_ASSERT_CAMPAIGN_LAYOUT_FLAG_INFO("flag_fourth", 7, 0x1 << 0);

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_byte_info(layoutPtr, "flag_first", &byteInfoBuffer));
    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_dynamic_info(layoutPtr, "flag_first", &dynamicInfoBuffer));

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_flag_info(layoutPtr, "byte_first", &flagInfoBuffer));
    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_dynamic_info(layoutPtr, "byte_first", &dynamicInfoBuffer));

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_flag_info(layoutPtr, "dynamic_int", &flagInfoBuffer));
    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_byte_info(layoutPtr, "dynamic_int", &byteInfoBuffer));

    CLEAR_CAMPAIGN_LAYOUT_TEST();
}

void test_campaign_layout_entry_exists(void)
{
    INIT_CAMPAIGN_LAYOUT_TEST(3);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists(layoutPtr, "flag_first", &existsBuffer));
    TEST_ASSERT_FALSE(existsBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_first"));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists(layoutPtr, "flag_first", &existsBuffer));
    TEST_ASSERT_TRUE(existsBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists(layoutPtr, "byte_first", &existsBuffer));
    TEST_ASSERT_FALSE(existsBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte(layoutPtr, "byte_first"));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists(layoutPtr, "byte_first", &existsBuffer));
    TEST_ASSERT_TRUE(existsBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists(layoutPtr, "dynamic_int", &existsBuffer));
    TEST_ASSERT_FALSE(existsBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic(layoutPtr, "dynamic_int", 4));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists(layoutPtr, "dynamic_int", &existsBuffer));
    TEST_ASSERT_TRUE(existsBuffer);

    CLEAR_CAMPAIGN_LAYOUT_TEST();
}

void test_campaign_layout_entry_exists_of_type(void)
{
    INIT_CAMPAIGN_LAYOUT_TEST(3);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(layoutPtr, "flag_first", CAMPAIGN_INFO_TYPE_FLAG, &existsBuffer));
    TEST_ASSERT_FALSE(existsBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_first"));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(layoutPtr, "flag_first", CAMPAIGN_INFO_TYPE_FLAG, &existsBuffer));
    TEST_ASSERT_TRUE(existsBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(layoutPtr, "flag_first", CAMPAIGN_INFO_TYPE_BYTE, &existsBuffer));
    TEST_ASSERT_FALSE(existsBuffer);
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(layoutPtr, "flag_first", CAMPAIGN_INFO_TYPE_DYNAMIC, &existsBuffer));
    TEST_ASSERT_FALSE(existsBuffer);
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(layoutPtr, "flag_first", CAMPAIGN_INFO_TYPE_VOID, &existsBuffer));
    TEST_ASSERT_FALSE(existsBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(layoutPtr, "byte_first", CAMPAIGN_INFO_TYPE_BYTE, &existsBuffer));
    TEST_ASSERT_FALSE(existsBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte(layoutPtr, "byte_first"));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(layoutPtr, "byte_first", CAMPAIGN_INFO_TYPE_BYTE, &existsBuffer));
    TEST_ASSERT_TRUE(existsBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(layoutPtr, "byte_first", CAMPAIGN_INFO_TYPE_FLAG, &existsBuffer));
    TEST_ASSERT_FALSE(existsBuffer);
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(layoutPtr, "byte_first", CAMPAIGN_INFO_TYPE_DYNAMIC, &existsBuffer));
    TEST_ASSERT_FALSE(existsBuffer);
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(layoutPtr, "byte_first", CAMPAIGN_INFO_TYPE_VOID, &existsBuffer));
    TEST_ASSERT_FALSE(existsBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(layoutPtr, "dynamic_int", CAMPAIGN_INFO_TYPE_DYNAMIC, &existsBuffer));
    TEST_ASSERT_FALSE(existsBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic(layoutPtr, "dynamic_int", 4));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(layoutPtr, "dynamic_int", CAMPAIGN_INFO_TYPE_DYNAMIC, &existsBuffer));
    TEST_ASSERT_TRUE(existsBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(layoutPtr, "dynamic_int", CAMPAIGN_INFO_TYPE_FLAG, &existsBuffer));
    TEST_ASSERT_FALSE(existsBuffer);
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(layoutPtr, "dynamic_int", CAMPAIGN_INFO_TYPE_BYTE, &existsBuffer));
    TEST_ASSERT_FALSE(existsBuffer);
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(layoutPtr, "dynamic_int", CAMPAIGN_INFO_TYPE_VOID, &existsBuffer));
    TEST_ASSERT_FALSE(existsBuffer);

    CLEAR_CAMPAIGN_LAYOUT_TEST();
}

void test_campaign_layout_get_entry_count(void)
{
    INIT_CAMPAIGN_LAYOUT_TEST(3);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_entry_count(layoutPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(0, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_first"));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_entry_count(layoutPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(1, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte(layoutPtr, "byte_first"));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_entry_count(layoutPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(2, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic(layoutPtr, "dynamic_int", 4));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_entry_count(layoutPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(3, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag_empty(layoutPtr));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_entry_count(layoutPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(3, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte_empty(layoutPtr));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_entry_count(layoutPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(3, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic_empty(layoutPtr, 4));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_entry_count(layoutPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(3, sizeBuffer);

    CLEAR_CAMPAIGN_LAYOUT_TEST();
}

void test_campaign_layout_get_data_size(void)
{
    INIT_CAMPAIGN_LAYOUT_TEST(5);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_data_size(layoutPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(0, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_first"));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_data_size(layoutPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(1, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_second"));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_data_size(layoutPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(1, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte(layoutPtr, "byte_first"));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_data_size(layoutPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(2, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_third"));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_data_size(layoutPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(3, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic(layoutPtr, "dynamic_int", 4));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_data_size(layoutPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(7, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag_empty(layoutPtr));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_data_size(layoutPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(8, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte_empty(layoutPtr));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_data_size(layoutPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(9, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic_empty(layoutPtr, 4));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_data_size(layoutPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(13, sizeBuffer);

    CLEAR_CAMPAIGN_LAYOUT_TEST();
}

void test_campaign_layout_get_entry_info(void)
{
    INIT_CAMPAIGN_LAYOUT_TEST(5);

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_entry_info(layoutPtr, 0, &entryInfoBuffer));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_first"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_second"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte(layoutPtr, "byte_first"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_third"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic(layoutPtr, "dynamic_int", 4));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_entry_count(layoutPtr, &sizeBuffer));

    size_t matchCount = 0;
    for (int32_t i = 0; i < sizeBuffer; i++)
    {
        size_t startingMatchCount = matchCount;

        TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_entry_info(layoutPtr, i, &entryInfoBuffer));

        if (strcmp(entryInfoBuffer.id, "flag_first") == 0)
        {
            matchCount++;

            TEST_ASSERT_EQUAL(CAMPAIGN_INFO_TYPE_FLAG, entryInfoBuffer.descriptor.type);
            TEST_ASSERT_EQUAL_INT32(0, entryInfoBuffer.descriptor.index);
            TEST_ASSERT_EQUAL_UINT8(0x1 << 0, entryInfoBuffer.descriptor.flagMask);
        }

        if (strcmp(entryInfoBuffer.id, "flag_second") == 0)
        {
            matchCount++;

            TEST_ASSERT_EQUAL(CAMPAIGN_INFO_TYPE_FLAG, entryInfoBuffer.descriptor.type);
            TEST_ASSERT_EQUAL_INT32(0, entryInfoBuffer.descriptor.index);
            TEST_ASSERT_EQUAL_UINT8(0x1 << 1, entryInfoBuffer.descriptor.flagMask);
        }

        if (strcmp(entryInfoBuffer.id, "byte_first") == 0)
        {
            matchCount++;

            TEST_ASSERT_EQUAL(CAMPAIGN_INFO_TYPE_BYTE, entryInfoBuffer.descriptor.type);
            TEST_ASSERT_EQUAL_INT32(1, entryInfoBuffer.descriptor.index);
        }

        if (strcmp(entryInfoBuffer.id, "flag_third") == 0)
        {
            matchCount++;

            TEST_ASSERT_EQUAL(CAMPAIGN_INFO_TYPE_FLAG, entryInfoBuffer.descriptor.type);
            TEST_ASSERT_EQUAL_INT32(2, entryInfoBuffer.descriptor.index);
            TEST_ASSERT_EQUAL_UINT8(0x1 << 0, entryInfoBuffer.descriptor.flagMask);
        }

        if (strcmp(entryInfoBuffer.id, "dynamic_int") == 0)
        {
            matchCount++;

            TEST_ASSERT_EQUAL(CAMPAIGN_INFO_TYPE_DYNAMIC, entryInfoBuffer.descriptor.type);
            TEST_ASSERT_EQUAL_INT32(3, entryInfoBuffer.descriptor.index);
            TEST_ASSERT_EQUAL_INT32(4, entryInfoBuffer.descriptor.size);
        }

        if (matchCount != (startingMatchCount + 1))
        {
            char message[256] = "";

            strcat(message, "Didn't expect an entry of id \"");
            strcat(message, entryInfoBuffer.id);
            strcat(message, "\".");

            TEST_FAIL_MESSAGE(message);
        }

        TEST_ASSERT_CAMPAIGN_API(campaign_string_free(entryInfoBuffer.id));
    }

    TEST_ASSERT_EQUAL_size_t(sizeBuffer, matchCount);

    CLEAR_CAMPAIGN_LAYOUT_TEST();
}

void test_campaign_layout_get_flag_info(void)
{
    INIT_CAMPAIGN_LAYOUT_TEST(5);

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_flag_info(layoutPtr, "flag_first", &flagInfoBuffer));
    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_flag_info(layoutPtr, "flag_second", &flagInfoBuffer));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_first"));

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_flag_info(layoutPtr, "flag_second", &flagInfoBuffer));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_second"));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_flag_info(layoutPtr, "flag_first", &flagInfoBuffer));
    TEST_ASSERT_EQUAL_INT32(0, flagInfoBuffer.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 0, flagInfoBuffer.flagMask);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_flag_info(layoutPtr, "flag_second", &flagInfoBuffer));
    TEST_ASSERT_EQUAL_INT32(0, flagInfoBuffer.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 1, flagInfoBuffer.flagMask);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte_empty(layoutPtr));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_third"));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_flag_info(layoutPtr, "flag_third", &flagInfoBuffer));
    TEST_ASSERT_EQUAL_INT32(2, flagInfoBuffer.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 0, flagInfoBuffer.flagMask);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte(layoutPtr, "byte"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic(layoutPtr, "dynamic", 4));

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_flag_info(layoutPtr, "byte", &flagInfoBuffer));
    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_flag_info(layoutPtr, "dynamic", &flagInfoBuffer));

    CLEAR_CAMPAIGN_LAYOUT_TEST();
}

void test_campaign_layout_get_byte_info(void)
{
    INIT_CAMPAIGN_LAYOUT_TEST(4);

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_byte_info(layoutPtr, "byte_first", &byteInfoBuffer));
    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_byte_info(layoutPtr, "byte_second", &byteInfoBuffer));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte(layoutPtr, "byte_first"));

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_byte_info(layoutPtr, "byte_second", &byteInfoBuffer));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte(layoutPtr, "byte_second"));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_byte_info(layoutPtr, "byte_first", &byteInfoBuffer));
    TEST_ASSERT_EQUAL_INT32(0, byteInfoBuffer.index);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_byte_info(layoutPtr, "byte_second", &byteInfoBuffer));
    TEST_ASSERT_EQUAL_INT32(1, byteInfoBuffer.index);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic(layoutPtr, "dynamic", 4));

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_byte_info(layoutPtr, "flag", &byteInfoBuffer));
    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_byte_info(layoutPtr, "dynamic", &byteInfoBuffer));

    CLEAR_CAMPAIGN_LAYOUT_TEST();
}

void test_campaign_layout_get_dynamic_info(void)
{
    INIT_CAMPAIGN_LAYOUT_TEST(4);

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_dynamic_info(layoutPtr, "dynamic_int", &dynamicInfoBuffer));
    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_dynamic_info(layoutPtr, "dynamic_double", &dynamicInfoBuffer));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic(layoutPtr, "dynamic_int", 4));

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_dynamic_info(layoutPtr, "dynamic_double", &dynamicInfoBuffer));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic(layoutPtr, "dynamic_double", 8));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_dynamic_info(layoutPtr, "dynamic_int", &dynamicInfoBuffer));
    TEST_ASSERT_EQUAL_INT32(0, dynamicInfoBuffer.index);
    TEST_ASSERT_EQUAL_INT32(4, dynamicInfoBuffer.size);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_dynamic_info(layoutPtr, "dynamic_double", &dynamicInfoBuffer));
    TEST_ASSERT_EQUAL_INT32(4, dynamicInfoBuffer.index);
    TEST_ASSERT_EQUAL_INT32(8, dynamicInfoBuffer.size);

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_dynamic_info(layoutPtr, "flag", &dynamicInfoBuffer));
    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_dynamic_info(layoutPtr, "byte", &dynamicInfoBuffer));

    CLEAR_CAMPAIGN_LAYOUT_TEST();
}

void test_campaign_layout(void)
{
    RUN_TEST(test_campaign_layout_create_delete);
    RUN_TEST(test_campaign_layout_flag_push);
    RUN_TEST(test_campaign_layout_flag_push_empty);
    RUN_TEST(test_campaign_layout_byte_push);
    RUN_TEST(test_campaign_layout_byte_push_empty);
    RUN_TEST(test_campaign_layout_dynamic_push);
    RUN_TEST(test_campaign_layout_dynamic_push_empty);
    RUN_TEST(test_campaign_layout_mixed_push);
    RUN_TEST(test_campaign_layout_entry_exists);
    RUN_TEST(test_campaign_layout_entry_exists_of_type);
    RUN_TEST(test_campaign_layout_get_entry_count);
    RUN_TEST(test_campaign_layout_get_data_size);
    RUN_TEST(test_campaign_layout_get_entry_info);
    RUN_TEST(test_campaign_layout_get_flag_info);
    RUN_TEST(test_campaign_layout_get_byte_info);
    RUN_TEST(test_campaign_layout_get_dynamic_info);
}