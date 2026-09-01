#include <campaign/api/api_layout.h>
#include <campaign/layout.hpp>
#include <iostream>

#if defined(_WIN32)
#include <objbase.h>
#endif

using namespace campaign;

#define TO_LAYOUT_PTR(ptr) static_cast<Layout *>(ptr)
#define TO_VOID_PTR(ptr) static_cast<void *>(ptr)

#if defined(_WIN32) // Windows .NET assumed COM allocator.
#define ALLOCATE_STRING(length) static_cast<char *>(CoTaskMemAlloc(sizeof(char) * length))
#else
#define ALLOCATE_STRING(length) static_cast<char *>(malloc(sizeof(char) * length))
#endif

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

void *campaign_layout_create(int32_t descriptorCount)
{
    return TO_VOID_PTR(new Layout(descriptorCount));
}

void campaign_layout_delete(void *layoutPtr)
{
    delete TO_LAYOUT_PTR(layoutPtr);
}

void campaign_layout_push_flag(void *layoutPtr, const char *id, int32_t idLength)
{
    TO_LAYOUT_PTR(layoutPtr)->pushFlag(std::string(id, idLength));
}

void campaign_layout_push_flag_empty(void *layoutPtr)
{
    TO_LAYOUT_PTR(layoutPtr)->pushFlag();
}

void campaign_layout_push_byte(void *layoutPtr, const char *id, int32_t idLength)
{
    TO_LAYOUT_PTR(layoutPtr)->pushByte(std::string(id, idLength));
}

void campaign_layout_push_byte_empty(void *layoutPtr)
{
    TO_LAYOUT_PTR(layoutPtr)->pushByte();
}

void campaign_layout_push_dynamic(void *layoutPtr, const char *id, int32_t idLength, int32_t size)
{
    TO_LAYOUT_PTR(layoutPtr)->pushDynamic(std::string(id, idLength), size);
}

void campaign_layout_push_dynamic_empty(void *layoutPtr, int32_t size)
{
    TO_LAYOUT_PTR(layoutPtr)->pushDynamic(size);
}

bool campaign_layout_entry_exists(void *layoutPtr, const char *id, int32_t idLength)
{
    return TO_LAYOUT_PTR(layoutPtr)->entryExists(std::string(id, idLength));
}

campaign_layout_entry_info_t campaign_layout_get_entry_info(void *layoutPtr, int32_t index)
{
    auto entryInfo = TO_LAYOUT_PTR(layoutPtr)->getEntryInfo(index);

    campaign_layout_entry_info_t c_entryInfo;
    c_entryInfo.idLength = entryInfo.id.length();
    c_entryInfo.id = ALLOCATE_STRING(c_entryInfo.idLength);

    switch(entryInfo.descriptor.getType())
    {
        case Descriptor::Type::Flag:
            SetDescriptorFlag(c_entryInfo.descriptor, entryInfo.descriptor.getFlagInfo());
            break;

        case Descriptor::Type::Byte:
            SetDescriptorByte(c_entryInfo.descriptor, entryInfo.descriptor.getByteInfo());
            break;

        case Descriptor::Type::Dynamic:
            SetDescriptorDynamic(c_entryInfo.descriptor, entryInfo.descriptor.getDynamicInfo());
            break;

        default:
            SetDescriptorVoid(c_entryInfo.descriptor);
            break;
    }

    return c_entryInfo;
}

campaign_descriptor_t campaign_layout_get_descriptor(void *layoutPtr, const char *id, int32_t idLength)
{
    auto descriptor = TO_LAYOUT_PTR(layoutPtr)->getDescriptor(std::string(id, idLength));

    campaign_descriptor_t c_descriptor;

    switch(descriptor.getType())
    {
        case Descriptor::Type::Flag:
            SetDescriptorFlag(c_descriptor, descriptor.getFlagInfo());
            break;

        case Descriptor::Type::Byte:
            SetDescriptorByte(c_descriptor, descriptor.getByteInfo());
            break;

        case Descriptor::Type::Dynamic:
            SetDescriptorDynamic(c_descriptor, descriptor.getDynamicInfo());
            break;

        default:
            SetDescriptorVoid(c_descriptor);
            break;
    }

    return c_descriptor;
}

campaign_flag_info_t campaign_layout_get_flag_info(void *layoutPtr, const char *id, int32_t idLength)
{
    auto flagInfo = TO_LAYOUT_PTR(layoutPtr)->getFlagInfo(std::string(id, idLength));

    campaign_flag_info_t c_flagInfo;
    c_flagInfo.index = flagInfo.index;
    c_flagInfo.flagMask = flagInfo.flagMask;

    return c_flagInfo;
}

campaign_byte_info_t campaign_layout_get_byte_info(void *layoutPtr, const char *id, int32_t idLength)
{
    auto byteInfo = TO_LAYOUT_PTR(layoutPtr)->getByteInfo(std::string(id, idLength));

    campaign_byte_info_t c_byteInfo;
    c_byteInfo.index = byteInfo.index;

    return c_byteInfo;
}

campaign_dynamic_info_t campaign_layout_get_dynamic_info(void *layoutPtr, const char *id, int32_t idLength, int32_t expectedSize)
{
    auto dynamicInfo = TO_LAYOUT_PTR(layoutPtr)->getDynamicInfo(std::string(id, idLength));

    campaign_dynamic_info_t c_dynamicInfo;
    c_dynamicInfo.index = dynamicInfo.index;
    c_dynamicInfo.size = dynamicInfo.size;

    return c_dynamicInfo;
}
