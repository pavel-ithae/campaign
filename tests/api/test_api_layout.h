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

    // TODO: Implement rest.

    campaign_layout_delete(layoutPtr);
}

void test_campaign_layout(void)
{
    RUN_TEST(test_campaign_layout_create_delete);
    RUN_TEST(test_campaign_layout_flag_push);
}