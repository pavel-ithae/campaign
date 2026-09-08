#pragma once

#include "test_api_util.h"

#include <campaign/api/api_blueprint.h>
#include <campaign/api/api_layout.h>

void test_campaign_blueprint(void)
{
    void *blueprintPtr;
    void *blueprintIteratorPtr;
    int32_t sizeBuffer;
    uint8_t existsBuffer;

    void *layoutPtr_a;
    void *layoutPtr_b;

    TEST_ASSERT_CAMPAIGN_API(campaign_blueprint_create(4, &blueprintPtr));

    TEST_ASSERT_CAMPAIGN_API(campaign_blueprint_get_size(blueprintPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(0, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_create(3, &layoutPtr_a));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_flag(layoutPtr_a, "flag"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte(layoutPtr_a, "byte"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_dynamic(layoutPtr_a, "dynamic", 4));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_create(2, &layoutPtr_b));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte(layoutPtr_b, "b_00"));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_push_byte(layoutPtr_b, "b_01"));

    TEST_ASSERT_CAMPAIGN_API(campaign_blueprint_push(blueprintPtr, "layout_a", layoutPtr_a));
    TEST_ASSERT_CAMPAIGN_API(campaign_blueprint_get_size(blueprintPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(1, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_blueprint_push(blueprintPtr, "layout_b", layoutPtr_b));
    TEST_ASSERT_CAMPAIGN_API(campaign_blueprint_get_size(blueprintPtr, &sizeBuffer));
    TEST_ASSERT_EQUAL_INT32(2, sizeBuffer);

    TEST_ASSERT_CAMPAIGN_API(campaign_blueprint_get_iterator(blueprintPtr, &blueprintIteratorPtr));

    int32_t blueprintSize;
    TEST_ASSERT_CAMPAIGN_API(campaign_blueprint_get_size(blueprintPtr, &blueprintSize));

    size_t matchCount = 0;
    campaign_blueprint_piece_info_t pieceInfo;
    for (size_t i = 0; i < blueprintSize; i++)
    {
        TEST_ASSERT_CAMPAIGN_API(campaign_blueprint_iterator_next(blueprintPtr, &blueprintIteratorPtr, &pieceInfo));

        if (strcmp(pieceInfo.id, "layout_a") == 0)
        {
            TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(pieceInfo.layoutPtr, "flag", CAMPAIGN_INFO_TYPE_FLAG, &existsBuffer));
            TEST_ASSERT_TRUE(existsBuffer);

            TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(pieceInfo.layoutPtr, "byte", CAMPAIGN_INFO_TYPE_BYTE, &existsBuffer));
            TEST_ASSERT_TRUE(existsBuffer);

            TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(pieceInfo.layoutPtr, "dynamic", CAMPAIGN_INFO_TYPE_DYNAMIC, &existsBuffer));
            TEST_ASSERT_TRUE(existsBuffer);

            TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(pieceInfo.layoutPtr, "b_00", CAMPAIGN_INFO_TYPE_BYTE, &existsBuffer));
            TEST_ASSERT_FALSE(existsBuffer);

            TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(pieceInfo.layoutPtr, "b_01", CAMPAIGN_INFO_TYPE_BYTE, &existsBuffer));
            TEST_ASSERT_FALSE(existsBuffer);

            matchCount++;
        }

        if (strcmp(pieceInfo.id, "layout_b") == 0)
        {
            TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(pieceInfo.layoutPtr, "b_00", CAMPAIGN_INFO_TYPE_BYTE, &existsBuffer));
            TEST_ASSERT_TRUE(existsBuffer);

            TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(pieceInfo.layoutPtr, "b_01", CAMPAIGN_INFO_TYPE_BYTE, &existsBuffer));
            TEST_ASSERT_TRUE(existsBuffer);

            TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(pieceInfo.layoutPtr, "flag", CAMPAIGN_INFO_TYPE_FLAG, &existsBuffer));
            TEST_ASSERT_FALSE(existsBuffer);

            TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(pieceInfo.layoutPtr, "byte", CAMPAIGN_INFO_TYPE_BYTE, &existsBuffer));
            TEST_ASSERT_FALSE(existsBuffer);

            TEST_ASSERT_CAMPAIGN_API(campaign_layout_entry_exists_of_type(pieceInfo.layoutPtr, "dynamic", CAMPAIGN_INFO_TYPE_DYNAMIC, &existsBuffer));
            TEST_ASSERT_FALSE(existsBuffer);

            matchCount++;
        }

        TEST_ASSERT_CAMPAIGN_API(campaign_string_free(pieceInfo.id));
        TEST_ASSERT_CAMPAIGN_API(campaign_layout_delete(pieceInfo.layoutPtr));
    }

    TEST_ASSERT_CAMPAIGN_API_FAIL(campaign_blueprint_iterator_next(blueprintPtr, &blueprintIteratorPtr, &pieceInfo));

    TEST_ASSERT_CAMPAIGN_API(campaign_layout_delete(layoutPtr_b));
    TEST_ASSERT_CAMPAIGN_API(campaign_layout_delete(layoutPtr_a));

    TEST_ASSERT_CAMPAIGN_API(campaign_blueprint_delete(blueprintPtr));
}