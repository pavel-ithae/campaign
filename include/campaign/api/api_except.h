
#pragma once

#include <campaign/api/api_util.h>

#ifdef __cplusplus
extern "C"
{
#endif
    /// @attention This returns a `char*` copy that needs to handled by the caller of this function. Make sure to free it when it's no longer being used!
    CAMPAIGN_API char *campaign_exception_get_last_message();
#ifdef __cplusplus
}
#endif