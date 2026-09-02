
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
    void campaign_exception_push_message(char *message);

    /// @attention This returns a `char*` copy that needs to handled by the caller of this function. Make sure to free it when it's no longer being used!
    char *campaign_exception_get_last_message();
#ifdef __cplusplus
}
#endif