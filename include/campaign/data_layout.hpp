#pragma once

#include <campaign/data_descriptor.hpp>
#include <unordered_map>
#include <string>

namespace campaign
{
    class DataLayout
    {
    public:
        DataLayout(size_t descriptorCount);

        const DataDescriptor &operator[](const std::string &id) const;

        void pushFlag(const std::string &id);
        void pushFlag();

        void pushByte(const std::string &id);
        void pushByte();

        void pushDynamic(const std::string &id, size_t descriptorCount);
        void pushDyanmic(size_t descriptorCount);

        size_t getDataSize() const;
        
        DataDescriptor::FlagDescription getFlagDescription(const std::string &id) const;
        DataDescriptor::ByteDescription getByteDescription(const std::string &id) const;
        DataDescriptor::DynamicDescription getDynamicDescription(const std::string &id, size_t expectedSize) const;

    private:
        std::unordered_map<std::string, DataDescriptor> descriptorMap_;
        size_t cursor_;
        uint8_t flagCursor_;

        void stepFlag();
        void stepByte();
        void stepBlock(size_t count);
        void stepToFreeByte();
    };
}