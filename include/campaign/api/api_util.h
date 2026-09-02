#pragma once

#if defined(_WIN32)
#if CAMPAIGN_API_BUILD
#define CAMPAIGN_API __declspec(dllexport)
#else
#define CAMPAIGN_API __declspec(dllimport)
#endif
#elif defined(__GNUC__) || defined(__clang__)
#define CAMPAIGN_API __attribute__((visibility("default")))
#else
#define CAMPAIGN_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    typedef enum campaign_api_result_t
    {
        CAMPAIGN_API_RESULT_SUCCESS = 0,
        CAMPAIGN_API_RESULT_FAIL = -1
    } campaign_api_result_t;
#ifdef __cplusplus
}
#endif