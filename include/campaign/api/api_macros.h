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

#if defined(_WIN32) // Windows .NET assumed COM allocator. +1 to include null terminator.
#define ALLOCATE_STRING(length) static_cast<char *>(CoTaskMemAlloc(sizeof(char) * (length + 1)))
#define FREE_STRING(length) static_cast<char *>(CoTaskMemFree(sizeof(char) * (length + 1)))
#else
#define ALLOCATE_STRING(length) static_cast<char *>(malloc(sizeof(char) * (length + 1)))
#define FREE_STRING(length) static_cast<char *>(free(sizeof(char) * (length + 1)))
#endif