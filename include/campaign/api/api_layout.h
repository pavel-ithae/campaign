#pragma once

#include <campaign/api/api_macros.h>
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
        int32_t idLength;
        campaign_descriptor_t descriptor;
    } campaign_layout_entry_info_t;

    CAMPAIGN_API void *campaign_layout_create(int32_t descriptorCount);

    CAMPAIGN_API void campaign_layout_delete(void *layoutPtr);

    CAMPAIGN_API void campaign_layout_push_flag(void *layoutPtr, const char* id, int32_t idLength);

    CAMPAIGN_API void campaign_layout_push_flag_empty(void *layoutPtr);

    CAMPAIGN_API void campaign_layout_push_byte(void *layoutPtr, const char* id, int32_t idLength);

    CAMPAIGN_API void campaign_layout_push_byte_empty(void *layoutPtr);

    CAMPAIGN_API void campaign_layout_push_dynamic(void *layoutPtr, const char* id, int32_t idLength, int32_t size);

    CAMPAIGN_API void campaign_layout_push_dynamic_empty(void *layoutPtr, int32_t size);

    CAMPAIGN_API bool campaign_layout_entry_exists(void *layoutPtr, const char* id, int32_t idLength);

    CAMPAIGN_API int32_t campaign_layout_get_entry_count(void *layoutPtr);

    CAMPAIGN_API int32_t campaign_layout_get_data_size(void *layoutPtr);

    CAMPAIGN_API campaign_layout_entry_info_t campaign_layout_get_entry_info(void *layoutPtr, int32_t index);

    CAMPAIGN_API campaign_descriptor_t campaign_layout_get_descriptor(void *layoutPtr, const char* id, int32_t idLength);

    CAMPAIGN_API campaign_flag_info_t campaign_layout_get_flag_info(void *layoutPtr, const char* id, int32_t idLength);

    CAMPAIGN_API campaign_byte_info_t campaign_layout_get_byte_info(void *layoutPtr, const char* id, int32_t idLength);

    CAMPAIGN_API campaign_dynamic_info_t campaign_layout_get_dynamic_info(void *layoutPtr, const char* id, int32_t idLength, int32_t expectedSize);
#ifdef __cplusplus
}
#endif