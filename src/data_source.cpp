#include <campaign/data_source.hpp>

#if NDEBUG // We only check for bounds in debug builds.
#define ASSERT_BOUNDS(index)
#else
#include <cassert>
#define ASSERT_BOUNDS(index) assert((index) < data_.size())
#endif

using namespace campaign;

void DataSource::setBlock(size_t index, void *ptr, size_t range)
{
    for (size_t i; i < range; ++i)
    {
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-arith"
#endif
        setByte(index, *static_cast<uint8_t*>(ptr + i));
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
    }
}

void *DataSource::getBlock(size_t index, size_t range)
{
    ASSERT_BOUNDS(index + range);

    return &data_[index];
}

void DataSource::notifyByteUpdated(size_t index, uint8_t previous, uint8_t current) const
{
    updateEvent_.call(index, previous, current);
}

DataSourceEventListenerToken DataSource::registerFlagCallback(size_t index, uint8_t flagMask, const DataSourceFlagUpdatedHandler &handler)
{
    return updateEvent_.registerCallback([index, flagMask, handler](size_t eventIndex, uint8_t previous, uint8_t current)
                            { 
                                if (eventIndex != index)
                                {
                                    return;
                                }

                                if (((previous ^ current) & flagMask) == 0x0)
                                {
                                    return;
                                }

                                if ((current & flagMask) == flagMask)
                                {
                                    handler(false, true);
                                }
                                else
                                {
                                    handler(true, false);
                                } });
}

DataSourceEventListenerToken DataSource::registerByteCallback(size_t index, const DataSourceByteUpdatedHandler &handler)
{
    return updateEvent_.registerCallback([index, handler](size_t eventIndex, uint8_t previous, uint8_t current)
                            {
                            if (eventIndex != index)
                            {
                                return;
                            }

                            handler(previous, current); });
}

void DataSource::unregisterUpdateCallback(DataSourceEventHandlerKey key)
{
    updateEvent_.unregisterCallback(key);
}

DataSource::DataSource(int size)
{
    data_.resize(size);
}

void DataSource::init()
{
    std::fill(data_.begin(), data_.end(), 0);
}

void DataSource::init(uint8_t *copyPtr)
{
    data_.assign(copyPtr, copyPtr + data_.size());
}

bool DataSource::setFlag(size_t index, uint8_t flagMask, bool value)
{
    ASSERT_BOUNDS(index);

    if (value)
    {
        if ((data_[index] & flagMask) == flagMask)
        {
            return false;
        }

        uint8_t previous = data_[index];

        data_[index] |= flagMask;

        notifyByteUpdated(index, previous, data_[index]);

        return true;
    }
    else
    {
        if ((data_[index] & flagMask) == 0x0)
        {
            return false;
        }

        uint8_t previous = data_[index];

        data_[index] &= ~flagMask;

        notifyByteUpdated(index, previous, data_[index]);

        return true;
    }
}

bool DataSource::setByte(size_t index, uint8_t value)
{
    ASSERT_BOUNDS(index);

    if (data_[index] == value)
    {
        return false;
    }

    uint8_t previous = data_[index];

    data_[index] = value;

    notifyByteUpdated(index, previous, data_[index]);

    return true;
}

bool DataSource::getFlag(size_t index, uint8_t flagMask) const
{
    ASSERT_BOUNDS(index);

    return (data_[index] & flagMask) == flagMask;
}

uint8_t DataSource::getByte(size_t index) const
{
    ASSERT_BOUNDS(index);

    return data_[index];
}

std::size_t DataSource::getSize() const
{
    return data_.size();
}
