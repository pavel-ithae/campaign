#pragma once

#include <campaign/api/api_layout.h>
#include <unity.h>

#define TEST_ASSERT_CAMPAIGN_API(call) TEST_ASSERT(call == CAMPAIGN_API_RESULT_SUCCESS)
#define TEST_ASSERT_CAMPAIGN_API_FAIL(call) TEST_ASSERT(call == CAMPAIGN_API_RESULT_FAIL)

void test_campaign_layout_create_delete(void)
{
    void *layoutPtr;
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_create(4, &layoutPtr));

    TEST_ASSERT_NOT_NULL(layoutPtr);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_delete(layoutPtr));
}

void test_campaign_layout_flag_push(void)
{
    void *layoutPtr;
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_create(9, &layoutPtr));

    int32_t size;
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_data_size(layoutPtr, &size));
    TEST_ASSERT_EQUAL_size_t(0, size);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_first"));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_data_size(layoutPtr, &size));
    TEST_ASSERT_EQUAL_size_t(1, size);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_second"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_third"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_fourth"));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_data_size(layoutPtr, &size));
    TEST_ASSERT_EQUAL_size_t(1, size);

    campaign_flag_info_t flagInfo;
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_flag_info(layoutPtr, "flag_first", &flagInfo));

    TEST_ASSERT_EQUAL_INT32(0, flagInfo.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 0, flagInfo.flagMask);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_flag_info(layoutPtr, "flag_second", &flagInfo));

    TEST_ASSERT_EQUAL_INT32(0, flagInfo.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 1, flagInfo.flagMask);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_flag_info(layoutPtr, "flag_third", &flagInfo));

    TEST_ASSERT_EQUAL_INT32(0, flagInfo.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 2, flagInfo.flagMask);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_flag_info(layoutPtr, "flag_fourth", &flagInfo));

    TEST_ASSERT_EQUAL_INT32(0, flagInfo.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 3, flagInfo.flagMask);

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_layout_get_flag_info(layoutPtr, "flag_fifth", &flagInfo));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_fifth"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_flag_info(layoutPtr, "flag_fifth", &flagInfo));

    TEST_ASSERT_EQUAL_INT32(0, flagInfo.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 4, flagInfo.flagMask);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_sixth"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_flag_info(layoutPtr, "flag_sixth", &flagInfo));

    TEST_ASSERT_EQUAL_INT32(0, flagInfo.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 5, flagInfo.flagMask);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_seventh"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_flag_info(layoutPtr, "flag_seventh", &flagInfo));

    TEST_ASSERT_EQUAL_INT32(0, flagInfo.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 6, flagInfo.flagMask);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_eighth"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_flag_info(layoutPtr, "flag_eighth", &flagInfo));

    TEST_ASSERT_EQUAL_INT32(0, flagInfo.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 7, flagInfo.flagMask);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr, "flag_ninth"));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_data_size(layoutPtr, &size));
    TEST_ASSERT_EQUAL_INT32(2, size);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_flag_info(layoutPtr, "flag_ninth", &flagInfo));

    TEST_ASSERT_EQUAL_INT32(1, flagInfo.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 0, flagInfo.flagMask);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_get_flag_info(layoutPtr, "flag_first", &flagInfo));

    TEST_ASSERT_EQUAL_INT32(0, flagInfo.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 0, flagInfo.flagMask);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_delete(layoutPtr));
}

void test_campaign_layout(void)
{
    RUN_TEST(test_campaign_layout_create_delete);
    RUN_TEST(test_campaign_layout_flag_push);
}