#pragma once

#include <campaign/api/api_macros.h>
#include <stdint.h>

#if __cplusplus
extern "C"
{
#endif
    typedef struct campaign_flag_info_t
    {
        int32_t index;
        uint8_t flagMask;
    } campaign_flag_info_t;

    typedef struct campaign_byte_info_t
    {
        int32_t index;
    } campaign_byte_info_t;

    typedef struct campaign_dynamic_info_t
    {
        int32_t index;
        int32_t size;
    } campaign_dynamic_info_t;

    typedef enum campaign_info_type_t
    {
        CAMPAIGN_INFO_TYPE_VOID,
        CAMPAIGN_INFO_TYPE_FLAG,
        CAMPAIGN_INFO_TYPE_BYTE,
        CAMPAIGN_INFO_TYPE_DYNAMIC
    } campaign_info_type_t;

    typedef struct campaign_descriptor_t
    {
        campaign_info_type_t type;
        int32_t index;
        uint8_t flagMask;
        int32_t size;
    } campaign_descriptor_t;
#if __cplusplus
}
#endif