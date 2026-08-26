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