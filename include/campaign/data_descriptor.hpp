#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>

namespace campaign
{
    class DataDescriptor
    {
    public:
        enum class Type
        {
            Flag,
            Byte,
            Dynamic,
            Void
        };

        struct Description
        {
            virtual Type getType() const { return Type::Void; }
        };

        struct FlagDescription : Description
        {
            FlagDescription(size_t index, uint8_t flagMask)
            {
                FlagDescription::index = index;
                FlagDescription::flagMask = flagMask;
            }

            size_t index;
            uint8_t flagMask;

            Type getType() const override { return Type::Flag; }
        };

        struct ByteDescription : Description
        {
            ByteDescription(size_t index)
            {
                ByteDescription::index = index;
            }

            size_t index;

            Type getType() const override { return Type::Byte; }
        };

        struct DynamicDescription : Description
        {
            DynamicDescription(size_t index, size_t size)
            {
                DynamicDescription::index = index;
                DynamicDescription::size = size;
            }

            size_t index;
            size_t size;

            Type getType() const override { return Type::Dynamic; }
        };

        DataDescriptor();
        DataDescriptor(FlagDescription description);
        DataDescriptor(ByteDescription description);
        DataDescriptor(DynamicDescription description);

        Type getType() const;

        FlagDescription getFlagDescription() const;
        ByteDescription getByteDescription() const;
        DynamicDescription getDynamicDescription(size_t expectedSize) const;

    private:
        const Type type_;
        const std::shared_ptr<Description> descriptionPtr_;
    };
}