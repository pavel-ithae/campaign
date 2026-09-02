#include <campaign/api/api_string.h>
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

char *campaign_string_allocate(size_t length)
{
    return ALLOCATE_STRING(length);
}

void campaign_string_free(char *strPtr)
{
    return FREE_STRING(strPtr);
}
