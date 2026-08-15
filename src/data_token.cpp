#include <campaign/data_token.hpp>
#include <stdexcept>

using namespace campaign;

bool DataToken::isValid() const
{
    return sourcePtr_.use_count() > 0;
}

DataToken::DataToken(const std::weak_ptr<DataSource> &sourcePtr)
    : sourcePtr_(sourcePtr)
{
}

DataSource &DataToken::getSource() const
{
    if (!isValid())
    {
        throw std::runtime_error("Trying to get data from an expired token.");
    }

    return *sourcePtr_.lock();
}

DataFlagToken::DataFlagToken(const std::weak_ptr<DataSource> &sourcePtr, size_t index, uint8_t flagMask)
    : DataToken(sourcePtr), index_(index), flagMask_(flagMask)
{
}

void DataFlagToken::set(bool value) const
{
    getSource().setFlag(index_, flagMask_, value);
}

bool DataFlagToken::get() const
{
    return getSource().getFlag(index_, flagMask_);
}

DataSourceEventListenerToken DataFlagToken::registerCallback(const DataSourceFlagUpdatedHandler &handler)
{
    return getSource().registerFlagCallback(index_, flagMask_, handler);
}

campaign::DataByteToken::DataByteToken(const std::weak_ptr<DataSource> &sourcePtr, size_t index)
    : DataToken(sourcePtr), index_(index)
{
}

void DataByteToken::set(uint8_t byte) const
{
    getSource().setByte(index_, byte);
}

uint8_t DataByteToken::get() const
{
    return getSource().getByte(index_);
}

DataSourceEventListenerToken campaign::DataByteToken::registerCallback(const DataSourceByteUpdatedHandler &handler)
{
    return getSource().registerByteCallback(index_, handler);
}
