#include <campaign/data_locator.hpp>

using namespace campaign;

DataLocator::DataLocator(const Layout &layout, const std::weak_ptr<Proxy> &proxyPtr)
    : layout_(layout), proxyPtr_(proxyPtr)
{
}

void DataLocator::setFlag(std::string id, bool value)
{
    auto desc = layout_.getFlagInfo(id);

    proxyPtr_.lock()->setFlag(desc.index, desc.flagMask, value);
}

void campaign::DataLocator::setByte(std::string id, uint8_t value)
{
    auto desc = layout_.getByteInfo(id);

    proxyPtr_.lock()->setByte(desc.index, value);
}

bool campaign::DataLocator::getFlag(std::string id)
{
    auto desc = layout_.getFlagInfo(id);

    return proxyPtr_.lock()->getFlag(desc.index, desc.flagMask);
}

uint8_t campaign::DataLocator::getByte(std::string id)
{
    auto desc = layout_.getByteInfo(id);

    return proxyPtr_.lock()->getByte(desc.index);
}

ProxyFlagToken DataLocator::getFlagToken(std::string id)
{
    auto desc = layout_.getFlagInfo(id);

    return ProxyFlagToken(proxyPtr_, desc.index, desc.flagMask);
}

ProxyByteToken DataLocator::getByteToken(std::string id)
{
    auto desc = layout_.getByteInfo(id);

    return ProxyByteToken(proxyPtr_, desc.index);
}