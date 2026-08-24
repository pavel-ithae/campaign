#include <campaign/locator.hpp>

using namespace campaign;

Locator::Locator(const Layout &layout, const std::weak_ptr<Proxy> &proxyPtr)
    : layout_(layout), proxyPtr_(proxyPtr)
{
}

void Locator::setFlag(const std::string &id, bool value)
{
    auto info = layout_.getFlagInfo(id);

    proxyPtr_.lock()->setFlag(info.index, info.flagMask, value);
}

void Locator::setByte(const std::string &id, uint8_t value)
{
    auto info = layout_.getByteInfo(id);

    proxyPtr_.lock()->setByte(info.index, value);
}

EventListenerToken Locator::registerFlagCallback(const std::string &id, const ProxyFlagUpdatedHandler &handler)
{
    auto info = layout_.getFlagInfo(id);

    return proxyPtr_.lock()->registerFlagCallback(info.index, info.flagMask, handler);
}

EventListenerToken Locator::registerByteCallback(const std::string &id, const ProxyByteUpdatedHandler &handler)
{
    auto info = layout_.getByteInfo(id);

    return proxyPtr_.lock()->registerByteCallback(info.index, handler);
}

bool Locator::getFlag(const std::string &id)
{
    auto info = layout_.getFlagInfo(id);

    return proxyPtr_.lock()->getFlag(info.index, info.flagMask);
}

uint8_t Locator::getByte(const std::string &id)
{
    auto info = layout_.getByteInfo(id);

    return proxyPtr_.lock()->getByte(info.index);
}

ProxyFlagToken Locator::getFlagToken(const std::string &id)
{
    auto info = layout_.getFlagInfo(id);

    return ProxyFlagToken(proxyPtr_, info.index, info.flagMask);
}

ProxyByteToken Locator::getByteToken(const std::string &id)
{
    auto info = layout_.getByteInfo(id);

    return ProxyByteToken(proxyPtr_, info.index);
}