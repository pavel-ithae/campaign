#pragma once

#include <campaign/api/api_macros.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
    CAMPAIGN_API void *campaign_layout_create(int32_t descriptorCount);

    CAMPAIGN_API void campaign_layout_delete(void *layoutPtr);
#ifdef __cplusplus
}
#endif