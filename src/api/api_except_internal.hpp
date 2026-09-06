#pragma once

#include <stdexcept>
#include <string>

#define CAMPAIGN_API_BEGIN \
    try                    \
    {

#define CAMPAIGN_API_END                 \
    return CAMPAIGN_API_RESULT_SUCCESS;  \
    }                                    \
    catch (const std::exception &e)      \
    {                                    \
        PushExceptionMessage(e.what());  \
        return CAMPAIGN_API_RESULT_FAIL; \
    }

#define CAMPAIGN_API_END_WITH_SAFETY(safetyOp) \
    return CAMPAIGN_API_RESULT_SUCCESS;        \
    }                                          \
    catch (const std::exception &e)            \
    {                                          \
        safetyOp;                              \
        PushExceptionMessage(e.what());        \
        return CAMPAIGN_API_RESULT_FAIL;       \
    }

#define CAMPAIGN_API_VALIDATE_PTR(ptr)                         \
    if (ptr == nullptr)                                        \
    {                                                          \
        PushExceptionMessage("Pointer \"" #ptr "\" is null."); \
        return CAMPAIGN_API_RESULT_FAIL;                       \
    }

void PushExceptionMessage(const std::string &message);