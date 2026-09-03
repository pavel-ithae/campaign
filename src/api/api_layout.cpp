#include <campaign/api/api_layout.h>
#include <campaign/api/api_string.h>
#include <campaign/api/api_except.h>
#include <campaign/layout.hpp>
#include <cstring>
#include <iostream>

#if defined(_WIN32)
#include <objbase.h>
#endif

using namespace campaign;

#define TO_LAYOUT_PTR(ptr) static_cast<Layout *>(ptr)
#define TO_VOID_PTR(ptr) static_cast<void *>(ptr)

void SetDescriptorVoid(campaign_descriptor_t &c_descriptor)
{
    c_descriptor.type = CAMPAIGN_INFO_TYPE_VOID;
    c_descriptor.index = 0;
    c_descriptor.flagMask = 0;
    c_descriptor.size = 0;
}

void SetDescriptorFlag(campaign_descriptor_t &c_descriptor, const Descriptor::FlagInfo &flagInfo)
{
    c_descriptor.type = CAMPAIGN_INFO_TYPE_FLAG;
    c_descriptor.index = flagInfo.index;
    c_descriptor.flagMask = flagInfo.flagMask;
    c_descriptor.size = 1;
}

void SetDescriptorByte(campaign_descriptor_t &c_descriptor, const Descriptor::ByteInfo &byteInfo)
{
    c_descriptor.type = CAMPAIGN_INFO_TYPE_BYTE;
    c_descriptor.index = byteInfo.index;
    c_descriptor.flagMask = 255;
    c_descriptor.size = 1;
}

void SetDescriptorDynamic(campaign_descriptor_t &c_descriptor, const Descriptor::DynamicInfo &dynamicInfo)
{
    c_descriptor.type = CAMPAIGN_INFO_TYPE_VOID;
    c_descriptor.index = dynamicInfo.index;
    c_descriptor.flagMask = 255;
    c_descriptor.size = dynamicInfo.size;
}

campaign_api_result_t campaign_layout_create(int32_t descriptorCount, void **outLayoutPtr)
{
    CAMPAIGN_API_VALIDATE_OUTPUT_PTR(outLayoutPtr);

    CAMPAIGN_API_BEGIN

    *outLayoutPtr = TO_VOID_PTR(new Layout(descriptorCount));

    CAMPAIGN_API_END
}

campaign_api_result_t campaign_layout_delete(void *layoutPtr)
{
    CAMPAIGN_API_BEGIN
    delete TO_LAYOUT_PTR(layoutPtr);
    CAMPAIGN_API_END
}

campaign_api_result_t campaign_layout_push_flag(void *layoutPtr, const char *id)
{
    CAMPAIGN_API_BEGIN
    TO_LAYOUT_PTR(layoutPtr)->pushFlag(std::string(id));
    CAMPAIGN_API_END
}

campaign_api_result_t campaign_layout_push_flag_empty(void *layoutPtr)
{
    CAMPAIGN_API_BEGIN
    TO_LAYOUT_PTR(layoutPtr)->pushFlag();
    CAMPAIGN_API_END
}

campaign_api_result_t campaign_layout_push_byte(void *layoutPtr, const char *id)
{
    CAMPAIGN_API_BEGIN
    TO_LAYOUT_PTR(layoutPtr)->pushByte(std::string(id));
    CAMPAIGN_API_END
}

campaign_api_result_t campaign_layout_push_byte_empty(void *layoutPtr)
{
    CAMPAIGN_API_BEGIN
    TO_LAYOUT_PTR(layoutPtr)->pushByte();
    CAMPAIGN_API_END
}

campaign_api_result_t campaign_layout_push_dynamic(void *layoutPtr, const char *id, int32_t size)
{
    CAMPAIGN_API_BEGIN
    TO_LAYOUT_PTR(layoutPtr)->pushDynamic(std::string(id), size);
    CAMPAIGN_API_END
}

campaign_api_result_t campaign_layout_push_dynamic_empty(void *layoutPtr, int32_t size)
{
    CAMPAIGN_API_BEGIN
    TO_LAYOUT_PTR(layoutPtr)->pushDynamic(size);
    CAMPAIGN_API_END
}

campaign_api_result_t campaign_layout_entry_exists(void *layoutPtr, const char *id, bool *outExists)
{
    CAMPAIGN_API_VALIDATE_OUTPUT_PTR(outExists);

    CAMPAIGN_API_BEGIN

    *outExists = TO_LAYOUT_PTR(layoutPtr)->entryExists(std::string(id));

    CAMPAIGN_API_END_WITH_SAFETY(*outExists = false)
}

campaign_api_result_t campaign_layout_get_entry_count(void *layoutPtr, int32_t *outCount)
{
    CAMPAIGN_API_VALIDATE_OUTPUT_PTR(outCount);

    CAMPAIGN_API_BEGIN

    *outCount = TO_LAYOUT_PTR(layoutPtr)->getEntryCount();

    CAMPAIGN_API_END_WITH_SAFETY(*outCount = 0)
}

campaign_api_result_t campaign_layout_get_data_size(void *layoutPtr, int32_t *outSize)
{
    CAMPAIGN_API_VALIDATE_OUTPUT_PTR(outSize);

    CAMPAIGN_API_BEGIN

    *outSize = TO_LAYOUT_PTR(layoutPtr)->getDataSize();

    CAMPAIGN_API_END_WITH_SAFETY(*outSize = 0)
}

campaign_api_result_t campaign_layout_get_entry_info(void *layoutPtr, int32_t index, campaign_layout_entry_info_t *outEntryInfo)
{
    CAMPAIGN_API_VALIDATE_OUTPUT_PTR(outEntryInfo);

    bool stringAllocated = false; // Tracks if an string was allocated so that it can be freed in case of a failure.

    CAMPAIGN_API_BEGIN

    auto entryInfo = TO_LAYOUT_PTR(layoutPtr)->getEntryInfo(index);
    size_t entryInfoIdLength = entryInfo.id.length();

    outEntryInfo->id = campaign_string_allocate(entryInfo.id.length());
    stringAllocated = true;

    std::memcpy(outEntryInfo->id, entryInfo.id.begin().base(), entryInfoIdLength + 1); // +1 to include null terminator.

    switch(entryInfo.descriptor.getType())
    {
        case Descriptor::Type::Flag:
            SetDescriptorFlag(outEntryInfo->descriptor, entryInfo.descriptor.getFlagInfo());
            break;

        case Descriptor::Type::Byte:
            SetDescriptorByte(outEntryInfo->descriptor, entryInfo.descriptor.getByteInfo());
            break;

        case Descriptor::Type::Dynamic:
            SetDescriptorDynamic(outEntryInfo->descriptor, entryInfo.descriptor.getDynamicInfo());
            break;

        default:
            SetDescriptorVoid(outEntryInfo->descriptor);
            break;
    }

    CAMPAIGN_API_END_WITH_SAFETY({
        if (stringAllocated)
        {
            campaign_string_free(outEntryInfo->id); // Free string to avoid memory leak.
        }

        outEntryInfo->id = nullptr;
        SetDescriptorVoid(outEntryInfo->descriptor);
    })
}

campaign_api_result_t campaign_layout_get_descriptor(void *layoutPtr, const char *id, campaign_descriptor_t *outDescriptor)
{
    CAMPAIGN_API_VALIDATE_OUTPUT_PTR(outDescriptor);

    CAMPAIGN_API_BEGIN

    auto descriptor = TO_LAYOUT_PTR(layoutPtr)->getDescriptor(std::string(id));

    switch(descriptor.getType())
    {
        case Descriptor::Type::Flag:
            SetDescriptorFlag(*outDescriptor, descriptor.getFlagInfo());
            break;

        case Descriptor::Type::Byte:
            SetDescriptorByte(*outDescriptor, descriptor.getByteInfo());
            break;

        case Descriptor::Type::Dynamic:
            SetDescriptorDynamic(*outDescriptor, descriptor.getDynamicInfo());
            break;

        default:
            SetDescriptorVoid(*outDescriptor);
            break;
    }

    CAMPAIGN_API_END_WITH_SAFETY({
        SetDescriptorVoid(*outDescriptor);
    })
}

campaign_api_result_t campaign_layout_get_flag_info(void *layoutPtr, const char *id, campaign_flag_info_t *outFlagInfo)
{
    CAMPAIGN_API_VALIDATE_OUTPUT_PTR(outFlagInfo);

    CAMPAIGN_API_BEGIN

    auto flagInfo = TO_LAYOUT_PTR(layoutPtr)->getFlagInfo(std::string(id));

    outFlagInfo->index = flagInfo.index;
    outFlagInfo->flagMask = flagInfo.flagMask;

    CAMPAIGN_API_END_WITH_SAFETY({
        outFlagInfo->index = 0;
        outFlagInfo->flagMask = 0x0;
    })
}

campaign_api_result_t campaign_layout_get_byte_info(void *layoutPtr, const char *id, campaign_byte_info_t *outByteInfo)
{
    CAMPAIGN_API_VALIDATE_OUTPUT_PTR(outByteInfo);

    CAMPAIGN_API_BEGIN

    auto byteInfo = TO_LAYOUT_PTR(layoutPtr)->getByteInfo(std::string(id));

    outByteInfo->index = byteInfo.index;

    CAMPAIGN_API_END_WITH_SAFETY({
        outByteInfo->index = 0;
    })
}

campaign_api_result_t campaign_layout_get_dynamic_info(void *layoutPtr, const char *id, int32_t expectedSize, campaign_dynamic_info_t *outDynamicInfo)
{
    CAMPAIGN_API_VALIDATE_OUTPUT_PTR(outDynamicInfo);

    CAMPAIGN_API_BEGIN

    auto dynamicInfo = TO_LAYOUT_PTR(layoutPtr)->getDynamicInfo(std::string(id));

    outDynamicInfo->index = dynamicInfo.index;
    outDynamicInfo->size = dynamicInfo.size;

    CAMPAIGN_API_END_WITH_SAFETY({
        outDynamicInfo->index = 0;
        outDynamicInfo->size = 0;
    })
}
