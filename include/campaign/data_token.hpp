#pragma once

#include <campaign/proxy.hpp>

// TODO: Implement a way to automatically release the token when data source is removed.

namespace campaign
{
    class DataToken
    {
    public:
        bool isValid() const;

    protected:
        DataToken(const std::weak_ptr<Proxy> &proxyPtr);

        Proxy &getSource() const;

    private:
        const std::weak_ptr<Proxy> proxyPtr_;
    };

    class DataFlagToken : DataToken
    {
    public:
        DataFlagToken(const std::weak_ptr<Proxy> &sourcePtr, size_t index, uint8_t flagMask);

        void set(bool value) const;
        bool get() const;

        EventListenerToken registerCallback(const ProxyFlagUpdatedHandler &handler);

    private:
        const size_t index_;
        const uint8_t flagMask_;
    };

    class DataByteToken : DataToken
    {
    public:
        DataByteToken(const std::weak_ptr<Proxy> &sourcePtr, size_t index);

        void set(uint8_t byte) const;
        uint8_t get() const;

        EventListenerToken registerCallback(const ProxyByteUpdatedHandler &handler);

    private:
        const size_t index_;
    };

    template <typename T>
    class DataDynamicToken : DataToken
    {
    public:
        DataDynamicToken(const std::weak_ptr<Proxy> &sourcePtr, size_t index);

        void set(const T &value) const;
        const T &get() const;

    private:
        const size_t index_;
    };

#include "data_token.tpp"
}