#pragma once

#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif
    char *campaign_string_allocate(size_t length);
    
    void campaign_string_free(char *strPtr);
#ifdef __cplusplus
}
#endif