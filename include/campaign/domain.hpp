#pragma once

#include <campaign/proxy.hpp>
#include <campaign/locator.hpp>
#include <campaign/blueprint.hpp>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

namespace campaign
{
    class Domain
    {
    public:
        struct Entry
        {
            std::string id;
            Layout layout;
        };

        Domain(const Blueprint &blueprint);

        void setFlag(const std::string &locationID, const std::string &flagID, bool value);
        void setByte(const std::string &locationID, const std::string &byteID, std::uint8_t value);

        template <typename T>
        void set(const std::string &locationID, const std::string &dynamicID, const T &value);

        bool getFlag(const std::string &locationID, const std::string &flagID);
        std::uint8_t getByte(const std::string &locationID, const std::string &byteID);

        ProxyFlagToken getFlagToken(const std::string &locationID, const std::string &flagID);
        ProxyByteToken getByteToken(const std::string &locationID, const std::string &byteID);

        template <typename T>
        T get(const std::string &locationID, const std::string &dynamicID);
        
        template <typename T>
        ProxyDynamicToken<T> getDynamicToken(const std::string locationID, const std::string dynamicID);

    private:
        std::vector<uint8_t> data_;
        std::vector<std::shared_ptr<Proxy>> proxies_;
        std::unordered_map<std::string, Locator> locators_;
    };

#include "domain.tpp"
}