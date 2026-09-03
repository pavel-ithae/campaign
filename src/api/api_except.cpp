#include <campaign/api/api_except.h>
#include <campaign/api/api_string.h>
#include <string>
#include <cstring>

std::string lastExceptionMessage = "";

void campaign_exception_push_message(const char *message)
{
    lastExceptionMessage = std::string(message);
}

char *campaign_exception_get_last_message()
{
    auto msgPtr = campaign_string_allocate(lastExceptionMessage.length()); // Freeing this will be handled by the caller.
    std::memcpy(msgPtr, lastExceptionMessage.begin().base(), lastExceptionMessage.length() + 1);

    return msgPtr;
}

