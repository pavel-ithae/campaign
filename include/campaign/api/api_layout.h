#pragma once

#include <campaign/api/api_util.h>
#include <campaign/api/api_descriptor.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct campaign_layout_entry_info_t
    {
        char *id;
        campaign_descriptor_t descriptor;
    } campaign_layout_entry_info_t;

    CAMPAIGN_API campaign_api_result_t campaign_layout_create(int32_t descriptorCount, void **outLayoutPtr);

    CAMPAIGN_API campaign_api_result_t campaign_layout_delete(void *layoutPtr);

    CAMPAIGN_API campaign_api_result_t campaign_layout_push_flag(void *layoutPtr, const char* id);

    CAMPAIGN_API campaign_api_result_t campaign_layout_push_flag_empty(void *layoutPtr);

    CAMPAIGN_API campaign_api_result_t campaign_layout_push_byte(void *layoutPtr, const char* id);

    CAMPAIGN_API campaign_api_result_t campaign_layout_push_byte_empty(void *layoutPtr);

    CAMPAIGN_API campaign_api_result_t campaign_layout_push_dynamic(void *layoutPtr, const char* id, int32_t size);

    CAMPAIGN_API campaign_api_result_t campaign_layout_push_dynamic_empty(void *layoutPtr, int32_t size);

    CAMPAIGN_API campaign_api_result_t campaign_layout_entry_exists(void *layoutPtr, const char* id, bool *outExists);

    CAMPAIGN_API campaign_api_result_t campaign_layout_get_entry_count(void *layoutPtr, int32_t *outCount);

    CAMPAIGN_API campaign_api_result_t campaign_layout_get_data_size(void *layoutPtr, int32_t *outSize);

    /// @attention The `char* campaign_layout_entry_info_t.id` memory needs to handled by the caller of this function. Make sure to free it when it's no longer being used!
    CAMPAIGN_API campaign_api_result_t campaign_layout_get_entry_info(void *layoutPtr, int32_t index, campaign_layout_entry_info_t *outEntryInfo);

    CAMPAIGN_API campaign_api_result_t campaign_layout_get_descriptor(void *layoutPtr, const char* id, campaign_descriptor_t *outDescriptor);

    CAMPAIGN_API campaign_api_result_t campaign_layout_get_flag_info(void *layoutPtr, const char* id, campaign_flag_info_t *outFlagInfo);

    CAMPAIGN_API campaign_api_result_t campaign_layout_get_byte_info(void *layoutPtr, const char* id, campaign_byte_info_t *outByteInfo);

    CAMPAIGN_API campaign_api_result_t campaign_layout_get_dynamic_info(void *layoutPtr, const char* id, int32_t expectedSize, campaign_dynamic_info_t *outDynamicInfo);
#ifdef __cplusplus
}
#endif