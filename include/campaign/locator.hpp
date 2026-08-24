#pragma once

#include <campaign/layout.hpp>
#include <campaign/proxy.hpp>
#include <string>
#include <cinttypes>
#include <unordered_map>

namespace campaign
{
    class Locator
    {
    public:
        Locator(const Layout &layout, const std::weak_ptr<Proxy> &proxyPtr);

        void setFlag(const std::string &id, bool value);
        void setByte(const std::string &id, uint8_t value);

        template <typename T>
        void set(const std::string &id, const T &value);

        EventListenerToken registerFlagCallback(const std::string &id, const ProxyFlagUpdatedHandler &handler);
        EventListenerToken registerByteCallback(const std::string &id, const ProxyByteUpdatedHandler &handler);

        bool getFlag(const std::string &id);
        uint8_t getByte(const std::string &id);

        template <typename T>
        T get(const std::string &id);

        ProxyFlagToken getFlagToken(const std::string &id);
        ProxyByteToken getByteToken(const std::string &id);

        template <typename T>
        ProxyDynamicToken<T> getDynamicToken(const std::string &id);

    private:
        const Layout layout_;
        const std::weak_ptr<Proxy> proxyPtr_;
    };

#include "locator.tpp"
}