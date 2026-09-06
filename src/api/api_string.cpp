#include <campaign/api/api_string.h>

#include "api_string_internal.hpp"
#include "api_except_internal.hpp"

#include <cstdlib>

#if defined(_WIN32)
#include <objbase.h> // includes CoTaskMemAlloc & CoTaskMemFree
#endif

#if defined(_WIN32) // Windows .NET assumed COM allocator. +1 to include null terminator.
#define ALLOCATE_STRING(length) (char *)(CoTaskMemAlloc(sizeof(char) * (length + 1)))
#define FREE_STRING(length) CoTaskMemFree(strPtr)
#else
#define ALLOCATE_STRING(length) (char *)(malloc(sizeof(char) * (length + 1)))
#define FREE_STRING(strPtr) free(strPtr)
#endif

char *AllocateString(size_t length)
{
    return ALLOCATE_STRING(length);
}

campaign_api_result_t campaign_string_free(char *strPtr)
{
    CAMPAIGN_API_VALIDATE_PTR(strPtr);

    CAMPAIGN_API_BEGIN

    FREE_STRING(strPtr);

    CAMPAIGN_API_END
}
