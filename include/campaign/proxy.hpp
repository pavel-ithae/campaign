#pragma once

#if CAMPAIGN_BOUNDS_CHECK_DISABLED
#define CAMPAIGN_SOURCE_CHECK_BOUNDS 0
#else
#define CAMPAIGN_SOURCE_CHECK_BOUNDS 1
#endif

#include <campaign/event.hpp>
#include <cstdint>
#include <vector>
#include <span>

namespace campaign
{
    // == Declarations. ==
    using ProxyUpdateEvent = Event<size_t, uint8_t, uint8_t>; // <index, previous, current>
    using ProxyUpdateEventListener = EventListener<size_t, uint8_t, uint8_t>;

    using ProxyFlagUpdatedHandler = std::function<void(bool, bool)>;
    using ProxyByteUpdatedHandler = std::function<void(uint8_t, uint8_t)>;

#if CAMPAIGN_LIBRARY_TESTING
    namespace testing
    {
        class EventTestHelper;
    }
#endif


    // == Proxy. ==
    class Proxy
    {
    public:
        Proxy();
        Proxy(const std::span<uint8_t> &span);
        Proxy(uint8_t *first, uint8_t *last);

        bool setFlag(size_t index, uint8_t flagMask, bool value);
        bool setByte(size_t index, uint8_t value);

        template <typename T>
        void set(size_t index, const T &value)
        {
            setBlock(index, &value, sizeof(T));
        }

        EventListenerToken registerFlagCallback(size_t index, uint8_t flagMask, const ProxyFlagUpdatedHandler &handler);
        EventListenerToken registerByteCallback(size_t index, const ProxyByteUpdatedHandler &handler);

        void unregisterUpdateCallback(EventListenerKey key);

        bool getFlag(size_t index, uint8_t flagMask) const;
        uint8_t getByte(size_t index) const;

        template <typename T>
        T get(size_t index)
        {
            return *static_cast<T *>(getBlock(index, sizeof(T)));
        }

        std::size_t getSize() const;
        const std::uint8_t *getDataPtr() const;

    private:
        std::span<uint8_t> data_;

        ProxyUpdateEvent updateEvent_;

        void setBlock(size_t index, const void *ptr, size_t range);

        void notifyByteUpdated(size_t index, uint8_t previous, uint8_t current) const;

        void *getBlock(size_t index, size_t range);

#if CAMPAIGN_LIBRARY_TESTING
        friend class testing::EventTestHelper;
#endif
    };


    // == Tokens. ==
    class ProxyToken
    {
    public:
        bool isValid() const;

    protected:
        ProxyToken(const std::weak_ptr<Proxy> &proxyPtr);

        Proxy &getSource() const;

    private:
        std::weak_ptr<Proxy> proxyPtr_;
    };

    class ProxyFlagToken : public ProxyToken
    {
    public:
        ProxyFlagToken(const std::weak_ptr<Proxy> &proxyPtr, size_t index, uint8_t flagMask);

        void set(bool value) const;
        bool get() const;

        EventListenerToken registerCallback(const ProxyFlagUpdatedHandler &handler);

    private:
        size_t index_;
        uint8_t flagMask_;
    };

    class ProxyByteToken : public ProxyToken
    {
    public:
        ProxyByteToken(const std::weak_ptr<Proxy> &proxyPtr, size_t index);

        void set(uint8_t byte) const;
        uint8_t get() const;

        EventListenerToken registerCallback(const ProxyByteUpdatedHandler &handler);

    private:
        size_t index_;
    };

    template <typename T>
    class ProxyDynamicToken : public ProxyToken
    {
    public:
        ProxyDynamicToken(const std::weak_ptr<Proxy> &proxyPtr, size_t index);

        void set(const T &value) const;
        T get() const;

    private:
        size_t index_;
    };

#include "proxy.tpp"
}