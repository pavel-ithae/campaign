#pragma once

#include <campaign/api/api_layout.h>
#include <unity.h>

void test_campaign_layout_create_delete(void)
{
    void *layoutPtr = campaign_layout_create(4);

    TEST_ASSERT_NOT_NULL(layoutPtr);

    campaign_layout_delete(layoutPtr);
}

void test_campaign_layout_flag_push(void)
{
    void *layoutPtr = campaign_layout_create(9);

    TEST_ASSERT_EQUAL_size_t(0, campaign_layout_get_data_size(layoutPtr));

    campaign_layout_push_flag(layoutPtr, "flag_first");

    TEST_ASSERT_EQUAL_size_t(1, campaign_layout_get_data_size(layoutPtr));

    campaign_layout_push_flag(layoutPtr, "flag_second");
    campaign_layout_push_flag(layoutPtr, "flag_third");
    campaign_layout_push_flag(layoutPtr, "flag_fourth");

    TEST_ASSERT_EQUAL_size_t(1, campaign_layout_get_data_size(layoutPtr));

    campaign_descriptor_t descriptor = campaign_layout_get_descriptor(layoutPtr, "flag_first");

    TEST_ASSERT_EQUAL(CAMPAIGN_INFO_TYPE_FLAG, descriptor.type);
    TEST_ASSERT_EQUAL_INT32(0, descriptor.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 0, descriptor.flagMask);

    descriptor = campaign_layout_get_descriptor(layoutPtr, "flag_second");

    TEST_ASSERT_EQUAL_INT32(0, descriptor.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 1, descriptor.flagMask);

    descriptor = campaign_layout_get_descriptor(layoutPtr, "flag_third");

    TEST_ASSERT_EQUAL_INT32(0, descriptor.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 2, descriptor.flagMask);

    descriptor = campaign_layout_get_descriptor(layoutPtr, "flag_fourth");

    TEST_ASSERT_EQUAL_INT32(0, descriptor.index);
    TEST_ASSERT_EQUAL_UINT8(0x1 << 3, descriptor.flagMask);

    campaign_layout_delete(layoutPtr);
}

void test_campaign_layout(void)
{
    RUN_TEST(test_campaign_layout_create_delete);
    RUN_TEST(test_campaign_layout_flag_push);
}