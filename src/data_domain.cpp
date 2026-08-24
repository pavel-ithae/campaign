#include <campaign/data_domain.hpp>

using namespace campaign;

DataDomain::DataDomain(const std::vector<Entry> &entries)
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

ProxyFlagToken DataDomain::getFlagToken(const std::string &locationID, const std::string &flagID)
{
    return locators_.at(locationID).getFlagToken(flagID);
}

ProxyByteToken DataDomain::getByteToken(const std::string &locationID, const std::string &byteID)
{
    return locators_.at(locationID).getByteToken(byteID);
}
