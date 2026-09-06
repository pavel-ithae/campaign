#include <campaign/api/api_except.h>

#include "api_except_internal.hpp"
#include "api_string_internal.hpp"

#include <string>
#include <cstring>
#include <functional>

std::string lastExceptionMessage = "";

void PushExceptionMessage(const std::string &message)
{
    lastExceptionMessage = message;
}

char *campaign_exception_get_last_message()
{
    auto msgPtr = AllocateString(lastExceptionMessage.length()); // Freeing this will be handled by the caller.
    std::memcpy(msgPtr, lastExceptionMessage.begin().base(), lastExceptionMessage.length() + 1);

    return msgPtr;
}
