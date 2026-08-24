#include <campaign/domain.hpp>

using namespace campaign;

Domain::Domain(const std::vector<Entry> &entries)
{
    size_t sizeTotal = 0;
    for (auto e : entries)
    {
        sizeTotal += e.layout.getDataSize();
    }

    data_.resize(sizeTotal);
    std::fill(data_.begin(), data_.end(), 0);

    auto ptr = data_.begin();

    proxies_.reserve(entries.size());
    locators_.reserve(entries.size());

    for (auto e : entries)
    {
        auto endPtr = std::next(ptr, e.layout.getDataSize());

        proxies_.push_back(std::make_shared<Proxy>(ptr.base(), endPtr.base()));
        locators_.insert({e.id, Locator(e.layout, proxies_.back())});

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
