#include <campaign/layout.hpp>
#include <assert.h>

using namespace campaign;

using FlagInfo = Descriptor::FlagInfo;
using ByteInfo = Descriptor::ByteInfo;
using DynamicInfo = Descriptor::DynamicInfo;

Layout::Layout()
{
    cursor_ = 0;
    flagCursor_ = 0x1;
}

Layout::Layout(size_t size)
    : Layout()
{
    descriptorMap_.reserve(size);
}

const Descriptor &Layout::operator[](const std::string &id) const
{
    return descriptorMap_.at(id);
}

void Layout::pushFlag(const std::string &id)
{
    descriptorMap_.insert({id, Descriptor(FlagInfo(cursor_, flagCursor_))});

    stepFlag();
}

void Layout::pushFlag()
{
    stepFlag();
}

void Layout::pushByte(const std::string &id)
{
    stepToFreeByte();

    descriptorMap_.insert({id, Descriptor(ByteInfo(cursor_))});

    stepByte();
}

void Layout::pushByte()
{
    stepToFreeByte();

    stepByte();
}

void Layout::pushDynamic(const std::string &id, size_t size)
{
    stepToFreeByte();

    descriptorMap_.insert({id, Descriptor(DynamicInfo(cursor_, size))});

    stepBlock(size);
}

void Layout::pushDynamic(size_t size)
{
    stepToFreeByte();

    stepBlock(size);
}

size_t Layout::getDataSize() const
{
    if (flagCursor_ == 0x1)
    {
        return cursor_;
    }

    return cursor_ + 1;
}

Descriptor::FlagInfo Layout::getFlagInfo(const std::string &id) const
{
    return descriptorMap_.at(id).getFlagInfo();
}

Descriptor::ByteInfo Layout::getByteInfo(const std::string &id) const
{
    return descriptorMap_.at(id).getByteInfo();
}

Descriptor::DynamicInfo Layout::getDynamicInfo(const std::string &id, size_t expectedSize) const
{
    return descriptorMap_.at(id).getDynamicInfo(expectedSize);
}

void Layout::stepFlag()
{
    if (flagCursor_ == (0x1 << 7))
    {
        cursor_++;
        flagCursor_ = 0x1;
        return;
    }

    flagCursor_ <<= 1;
}

void Layout::stepByte()
{
    flagCursor_ = 0x1;
    cursor_++;
}

void Layout::stepBlock(size_t count)
{
    flagCursor_ = 0x1;
    cursor_ += count;
}

void Layout::stepToFreeByte()
{
    if (flagCursor_ == 0x1)
    {
        return;
    }

    stepByte();
}
