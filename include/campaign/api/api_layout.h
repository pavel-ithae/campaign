#pragma once

#include <campaign/api/api_macros.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif
    CAMPAIGN_API void *campaign_layout_create(size_t descriptorCount);

    CAMPAIGN_API void campaign_layout_delete(void *layoutPtr);
#ifdef __cplusplus
}
#endif