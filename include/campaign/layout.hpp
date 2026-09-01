#pragma once

#include <campaign/descriptor.hpp>
#include <vector>
#include <unordered_map>
#include <string>

namespace campaign
{
#if CAMPAIGN_LIBRARY_TESTING
    namespace testing
    {
        class LayoutTestHelper;
    }
#endif

    class Layout
    {
    public:
        struct EntryInfo
        {
            const std::string id;
            const Descriptor descriptor;

            EntryInfo(const std::string &id, const Descriptor &descriptor) : id(id), descriptor(descriptor) {}
        };

        Layout();
        Layout(size_t entryCount);

        const Descriptor &operator[](const std::string &id) const;

        void pushFlag(const std::string &id);
        void pushFlag();

        void pushByte(const std::string &id);
        void pushByte();

        void pushDynamic(const std::string &id, size_t size);
        void pushDynamic(size_t size);

        bool entryExists(const std::string id) const;

        size_t getEntryCount() const;
        size_t getDataSize() const;

        EntryInfo getEntryInfo(size_t index) const;
        const Descriptor &getDescriptor(const std::string &id) const;

        Descriptor::FlagInfo getFlagInfo(const std::string &id) const;
        Descriptor::ByteInfo getByteInfo(const std::string &id) const;
        Descriptor::DynamicInfo getDynamicInfo(const std::string &id) const;

    private:
        std::unordered_map<std::string, Descriptor> descriptorMap_;
        size_t cursor_;
        uint8_t flagCursor_;

        void stepFlag();
        void stepByte();
        void stepBlock(size_t count);
        void stepToFreeByte();

#if CAMPAIGN_LIBRARY_TESTING
        friend class testing::LayoutTestHelper;
#endif
    };
}