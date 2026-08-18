#include <campaign/data_source.hpp>

#if CAMPAIGN_SOURCE_CHECK_BOUNDS
#include <stdexcept>
#include <string>

#define CHECK_DATA_BOUNDS(index) tryCheckBounds(*this, index);
#else // !CAMPAIGN_SOURCE_CHECK_BOUNDS
#define CHECK_DATA_BOUNDS(index) 
#endif // CAMPAIGN_SOURCE_CHECK_BOUNDS

using namespace campaign;

#if CAMPAIGN_SOURCE_CHECK_BOUNDS
inline void tryCheckBounds(const DataSource& source, size_t index)
{
    if (index >= source.getSize())
    {
        throw std::out_of_range("Trying to access a value (at " + std::to_string(index) + ") outside of the source's range ("+ std::to_string(source.getSize()) + ").");
    }
}
#endif

DataSource::DataSource()
{
}

DataSource::DataSource(size_t size)
{
    data_.resize(size);
}

void DataSource::init()
{
    std::fill(data_.begin(), data_.end(), 0);
}

void DataSource::init(const uint8_t *copyPtr)
{
    data_.assign(copyPtr, copyPtr + data_.size());
}

bool DataSource::setFlag(size_t index, uint8_t flagMask, bool value)
{
    CHECK_DATA_BOUNDS(index);

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
    CHECK_DATA_BOUNDS(index);

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
    CHECK_DATA_BOUNDS(index);

    return (data_[index] & flagMask) == flagMask;
}

uint8_t DataSource::getByte(size_t index) const
{
    CHECK_DATA_BOUNDS(index);

    return data_[index];
}

std::size_t DataSource::getSize() const
{
    return data_.size();
}

const std::uint8_t *DataSource::getDataPtr() const
{
    return data_.begin().base();
}
void DataSource::setBlock(size_t index, const void *ptr, size_t range)
{
    CHECK_DATA_BOUNDS(index + (range - 1));

    for (size_t i = index; i < range; ++i)
    {
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-arith"
#endif
        data_[i] = *static_cast<const uint8_t*>(ptr + i);
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
    }
}

void *DataSource::getBlock(size_t index, size_t range)
{
    CHECK_DATA_BOUNDS(index + (range - 1));

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
