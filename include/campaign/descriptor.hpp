#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

namespace campaign
{
    class Descriptor
    {
    public:
        enum class Type
        {
            Void,
            Flag,
            Byte,
            Dynamic
        };

        struct Info
        {
            virtual Type getType() const { return Type::Void; }
        };

        struct FlagInfo : public Info
        {
            FlagInfo(size_t index, uint8_t flagMask)
            {
                FlagInfo::index = index;
                FlagInfo::flagMask = flagMask;
            }

            size_t index;
            uint8_t flagMask;

            Type getType() const override { return Type::Flag; }
        };

        struct ByteInfo : public Info
        {
            ByteInfo(size_t index)
            {
                ByteInfo::index = index;
            }

            size_t index;

            Type getType() const override { return Type::Byte; }
        };

        struct DynamicInfo : public Info
        {
            DynamicInfo(size_t index, size_t size)
            {
                DynamicInfo::index = index;
                DynamicInfo::size = size;
            }

            size_t index;
            size_t size;

            Type getType() const override { return Type::Dynamic; }
        };

        Descriptor();
        Descriptor(FlagInfo flagInfo);
        Descriptor(ByteInfo byteInfo);
        Descriptor(DynamicInfo dynamicInfo);

        Type getType() const;

        FlagInfo getFlagInfo() const;
        ByteInfo getByteInfo() const;
        DynamicInfo getDynamicInfo() const;

    private:
        const Type type_;
        const std::shared_ptr<Info> infoPtr_;
    };
}