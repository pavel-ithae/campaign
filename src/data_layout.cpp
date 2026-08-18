#include <campaign/data_layout.hpp>
#include <assert.h>

using namespace campaign;

using FlagDescription = DataDescriptor::FlagDescription;
using ByteDescription = DataDescriptor::ByteDescription;
using DynamicDescription = DataDescriptor::DynamicDescription;

DataLayout::DataLayout()
{
    cursor_ = 0;
    flagCursor_ = 0x1;
}

DataLayout::DataLayout(size_t size)
    : DataLayout()
{
    descriptorMap_.reserve(size);
}

const DataDescriptor &DataLayout::operator[](const std::string &id) const
{
    return descriptorMap_.at(id);
}

void DataLayout::pushFlag(const std::string &id)
{
    descriptorMap_.insert({id, DataDescriptor(FlagDescription(cursor_, flagCursor_))});

    stepFlag();
}

void DataLayout::pushFlag()
{
    stepFlag();
}

void DataLayout::pushByte(const std::string &id)
{
    stepToFreeByte();

    descriptorMap_.insert({id, DataDescriptor(ByteDescription(cursor_))});

    stepByte();
}

void DataLayout::pushByte()
{
    stepToFreeByte();

    stepByte();
}

void DataLayout::pushDynamic(const std::string &id, size_t size)
{
    stepToFreeByte();

    descriptorMap_.insert({id, DataDescriptor(DynamicDescription(cursor_, size))});

    stepBlock(size);
}

void DataLayout::pushDynamic(size_t size)
{
    stepToFreeByte();

    stepBlock(size);
}

size_t DataLayout::getDataSize() const
{
    if (flagCursor_ == 0x1)
    {
        return cursor_;
    }

    return cursor_ + 1;
}

DataDescriptor::FlagDescription DataLayout::getFlagDescription(const std::string &id) const
{
    return descriptorMap_.at(id).getFlagDescription();
}

DataDescriptor::ByteDescription DataLayout::getByteDescription(const std::string &id) const
{
    return descriptorMap_.at(id).getByteDescription();
}

DataDescriptor::DynamicDescription DataLayout::getDynamicDescription(const std::string &id, size_t expectedSize) const
{
    return descriptorMap_.at(id).getDynamicDescription(expectedSize);
}

void DataLayout::stepFlag()
{
    if (flagCursor_ == (0x1 << 7))
    {
        cursor_++;
        flagCursor_ = 0x1;
        return;
    }

    flagCursor_ <<= 1;
}

void DataLayout::stepByte()
{
    flagCursor_ = 0x1;
    cursor_++;
}

void DataLayout::stepBlock(size_t count)
{
    flagCursor_ = 0x1;
    cursor_ += count;
}

void DataLayout::stepToFreeByte()
{
    if (flagCursor_ == 0x1)
    {
        return;
    }

    stepByte();
}
