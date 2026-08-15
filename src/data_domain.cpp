#include <campaign/data_domain.hpp>

using namespace campaign;

DataDomain::DataDomain(const std::vector<Entry> &entries)
{
    sources_.reserve(entries.size());
    locators_.reserve(entries.size());

    for (auto e : entries)
    {
        sources_.push_back(std::make_shared<DataSource>(e.layout.getDataSize()));
        
        locators_.insert({e.id, DataLocator(e.layout, sources_.back())});
    }
}

DataFlagToken DataDomain::getFlagToken(const std::string &locationID, const std::string &flagID)
{
    return locators_.at(locationID).getFlagToken(flagID);
}

DataByteToken DataDomain::getByteToken(const std::string &locationID, const std::string &byteID)
{
    return locators_.at(locationID).getByteToken(byteID);
}
