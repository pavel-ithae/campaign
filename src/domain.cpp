#include <campaign/domain.hpp>
#include <span>

using namespace campaign;

Domain::Domain(const Blueprint &blueprint)
{
    size_t sizeTotal = 0;
    for (auto p : blueprint)
    {
        sizeTotal += p.layout.getDataSize();
    }

    data_.resize(sizeTotal);
    std::fill(data_.begin(), data_.end(), 0);

    auto ptr = data_.begin();

    proxies_.reserve(blueprint.size());
    locators_.reserve(blueprint.size());

    for (auto p : blueprint)
    {
        auto endPtr = std::next(ptr, p.layout.getDataSize());

        proxies_.push_back(std::make_shared<Proxy>(ptr.base(), endPtr.base()));
        locators_.insert({p.id, Locator(p.layout, proxies_.back())});

        ptr = endPtr;
    }
}

void Domain::setFlag(const std::string &locationID, const std::string &flagID, bool value)
{
    locators_.at(locationID).setFlag(flagID, value);
}

void Domain::setByte(const std::string &locationID, const std::string &byteID, std::uint8_t value)
{
    locators_.at(locationID).setByte(byteID, value);
}

bool Domain::getFlag(const std::string &locationID, const std::string &flagID)
{
    return locators_.at(locationID).getFlag(flagID);
}

std::uint8_t Domain::getByte(const std::string &locationID, const std::string &byteID)
{
    return locators_.at(locationID).getByte(byteID);
}

ProxyFlagToken Domain::getFlagToken(const std::string &locationID, const std::string &flagID)
{
    return locators_.at(locationID).getFlagToken(flagID);
}

ProxyByteToken Domain::getByteToken(const std::string &locationID, const std::string &byteID)
{
    return locators_.at(locationID).getByteToken(byteID);
}
