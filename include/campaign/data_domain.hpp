#pragma once

#include <campaign/proxy.hpp>
#include <campaign/data_locator.hpp>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

namespace campaign
{
    class DataDomain
    {
    public:
        struct Entry
        {
            std::string id;
            Layout layout;
        };

        DataDomain(const std::vector<Entry> &entries);

        ProxyFlagToken getFlagToken(const std::string &locationID, const std::string &flagID);
        ProxyByteToken getByteToken(const std::string &locationID, const std::string &byteID);
        
        template <typename T>
        ProxyDynamicToken<T> getDynamicToken(const std::string locationID, const std::string dynamicID);

    private:
        std::vector<uint8_t> data_;
        std::vector<std::shared_ptr<Proxy>> proxies_;
        std::unordered_map<std::string, DataLocator> locators_;
    };

    #include "data_domain.tpp"
}