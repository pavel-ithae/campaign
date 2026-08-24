#include <campaign/proxy.hpp>

#if CAMPAIGN_SOURCE_CHECK_BOUNDS
#include <stdexcept>
#include <string>

#define CHECK_DATA_BOUNDS(index) tryCheckBounds(*this, index);
#else // !CAMPAIGN_SOURCE_CHECK_BOUNDS
#define CHECK_DATA_BOUNDS(index) 
#endif // CAMPAIGN_SOURCE_CHECK_BOUNDS

using namespace campaign;

#if CAMPAIGN_SOURCE_CHECK_BOUNDS
inline void tryCheckBounds(const Proxy& source, size_t index)
{
    if (index >= source.getSize())
    {
        throw std::out_of_range("Trying to access a value (at " + std::to_string(index) + ") outside of the source's range ("+ std::to_string(source.getSize()) + ").");
    }
}
#endif

Proxy::Proxy()
{
}

Proxy::Proxy(const std::span<uint8_t> &span)
{
    data_ = span;
}

Proxy::Proxy(uint8_t *first, uint8_t *last)
{
    data_ = std::span<uint8_t>(first, last);
}

bool Proxy::setFlag(size_t index, uint8_t flagMask, bool value)
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

bool Proxy::setByte(size_t index, uint8_t value)
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

bool Proxy::getFlag(size_t index, uint8_t flagMask) const
{
    CHECK_DATA_BOUNDS(index);

    return (data_[index] & flagMask) == flagMask;
}

uint8_t Proxy::getByte(size_t index) const
{
    CHECK_DATA_BOUNDS(index);

    return data_[index];
}

std::size_t Proxy::getSize() const
{
    return data_.size();
}

const std::uint8_t *Proxy::getDataPtr() const
{
    return data_.begin().base();
}
void Proxy::setBlock(size_t index, const void *ptr, size_t range)
{
    CHECK_DATA_BOUNDS(index + (range - 1));

    for (size_t i = 0; i < range; ++i)
    {
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-arith"
#endif
        data_[index + i] = *static_cast<const uint8_t*>(ptr + i);
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
    }
}

void *Proxy::getBlock(size_t index, size_t range)
{
    CHECK_DATA_BOUNDS(index + (range - 1));

    return &data_[index];
}

void Proxy::notifyByteUpdated(size_t index, uint8_t previous, uint8_t current) const
{
    updateEvent_.call(index, previous, current);
}

EventListenerToken Proxy::registerFlagCallback(size_t index, uint8_t flagMask, const ProxyFlagUpdatedHandler &handler)
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

EventListenerToken Proxy::registerByteCallback(size_t index, const ProxyByteUpdatedHandler &handler)
{
    return updateEvent_.registerCallback([index, handler](size_t eventIndex, uint8_t previous, uint8_t current)
                            {
                            if (eventIndex != index)
                            {
                                return;
                            }

                            handler(previous, current); });
}

void Proxy::unregisterUpdateCallback(EventListenerKey key)
{
    updateEvent_.unregisterCallback(key);
}

bool ProxyToken::isValid() const
{
    return proxyPtr_.use_count() > 0;
}

ProxyToken::ProxyToken(const std::weak_ptr<Proxy> &proxyPtr)
    : proxyPtr_(proxyPtr)
{
}

Proxy &ProxyToken::getSource() const
{
    if (!isValid())
    {
        throw std::runtime_error("Trying to get data from an expired token.");
    }

    return *proxyPtr_.lock();
}

ProxyFlagToken::ProxyFlagToken(const std::weak_ptr<Proxy> &proxyPtr, size_t index, uint8_t flagMask)
    : ProxyToken(proxyPtr), index_(index), flagMask_(flagMask)
{
}

void ProxyFlagToken::set(bool value) const
{
    getSource().setFlag(index_, flagMask_, value);
}

bool ProxyFlagToken::get() const
{
    return getSource().getFlag(index_, flagMask_);
}

EventListenerToken ProxyFlagToken::registerCallback(const ProxyFlagUpdatedHandler &handler)
{
    return getSource().registerFlagCallback(index_, flagMask_, handler);
}

campaign::ProxyByteToken::ProxyByteToken(const std::weak_ptr<Proxy> &sourcePtr, size_t index)
    : ProxyToken(sourcePtr), index_(index)
{
}

void ProxyByteToken::set(uint8_t byte) const
{
    getSource().setByte(index_, byte);
}

uint8_t ProxyByteToken::get() const
{
    return getSource().getByte(index_);
}

EventListenerToken campaign::ProxyByteToken::registerCallback(const ProxyByteUpdatedHandler &handler)
{
    return getSource().registerByteCallback(index_, handler);
}
