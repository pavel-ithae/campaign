
#pragma once

#include <campaign/api/api_util.h>

#ifdef __cplusplus
#include <stdexcept>
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    void campaign_exception_push_message(const char *message);

    /// @attention This returns a `char*` copy that needs to handled by the caller of this function. Make sure to free it when it's no longer being used!
    char *campaign_exception_get_last_message();
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#define CAMPAIGN_API_BEGIN \
    try                    \
    {

#define CAMPAIGN_API_END                           \
    return CAMPAIGN_API_RESULT_SUCCESS;            \
    }                                              \
    catch (const std::exception &e)                \
    {                                              \
        campaign_exception_push_message(e.what()); \
        return CAMPAIGN_API_RESULT_FAIL;           \
    }

#define CAMPAIGN_API_END_WITH_SAFETY(safetyOp)     \
    return CAMPAIGN_API_RESULT_SUCCESS;            \
    }                                              \
    catch (const std::exception &e)                \
    {                                              \
        safetyOp;                                  \
        campaign_exception_push_message(e.what()); \
        return CAMPAIGN_API_RESULT_FAIL;           \
    }

#define CAMPAIGN_API_VALIDATE_OUTPUT_PTR(ptr) if (ptr == nullptr) { campaign_exception_push_message("Output parameter is null."); return CAMPAIGN_API_RESULT_FAIL; }
#endif