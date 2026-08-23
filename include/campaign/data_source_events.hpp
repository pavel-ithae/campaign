#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>

namespace campaign
{
    class IDataSourceEvent; // Forward declaration for tokens.

#if CAMPAIGN_LIBRARY_TESTING
    namespace testing
    {
        class DataSourceEventTest;
    }
#endif

    using DataSourceEventHandlerKey = char *;

    class IDataSourceEventListener
    {
    protected:
        IDataSourceEventListener();

    public:
        IDataSourceEventListener(const IDataSourceEventListener &) = delete; // Copy constructor could lead to deleting the key early and double deleting after.

        virtual ~IDataSourceEventListener();

        IDataSourceEventListener &operator=(const IDataSourceEventListener &) = delete; // Copy assignment could lead to deleting the key early  and double deleting after.

        DataSourceEventHandlerKey getKey() const;

    private:
        DataSourceEventHandlerKey key_;
    };

    template <typename... TArgs>
    class DataSourceEventListener : public IDataSourceEventListener
    {
    public:
        DataSourceEventListener(const std::function<void(TArgs...)> &callback);
        DataSourceEventListener(const DataSourceEventListener<TArgs...> &) = delete; // Copy constructor could lead to deleting the key early and double deleting after.

        DataSourceEventListener<TArgs...> &operator=(const DataSourceEventListener<TArgs...> &) = delete; // Copy assignment could lead to deleting the key early  and double deleting after.

        void call(TArgs... args) const;

    private:
        std::function<void(TArgs...)> callback_;
    };

    /// @brief Token to keep track of a registered callback.
    class DataSourceEventListenerToken
    {
        // TODO: Implement a way to automatically release the token when data source is removed.
    public:
        DataSourceEventListenerToken(IDataSourceEvent &event, const std::shared_ptr<IDataSourceEventListener> &listener);

        template <typename... TArgs>
        DataSourceEventListenerToken(IDataSourceEvent &event, const std::shared_ptr<DataSourceEventListener<TArgs...>> listener);

        void unregister() const;

        bool isActive() const;

    private:
        IDataSourceEvent *event_;
        std::weak_ptr<IDataSourceEventListener> listener_;
    };

    /// @brief Base class for data source events.
    class IDataSourceEvent
    {
    protected:
        IDataSourceEvent() {}

    public:
        virtual void unregisterCallback(DataSourceEventHandlerKey key) = 0;
    };

    /// @brief Template for data source events.
    /// @tparam ...TArgs
    template <typename... TArgs>
    class DataSourceEvent : IDataSourceEvent
    {
    public:
        DataSourceEvent() : IDataSourceEvent() {}

        using Listener = DataSourceEventListener<TArgs...>;

        void call(TArgs... args) const;

        DataSourceEventListenerToken registerCallback(const std::function<void(TArgs...)> &handler);

        void unregisterCallback(DataSourceEventHandlerKey key) override;

    private:
        std::unordered_map<DataSourceEventHandlerKey, std::shared_ptr<Listener>> listenersMap_;

#if CAMPAIGN_LIBRARY_TESTING
        friend class testing::DataSourceEventTest;
#endif // CAMPAIGN_LIBRARY_TESTING
    };

    using DataSourceUpdateEvent = DataSourceEvent<size_t, uint8_t, uint8_t>; // <index, previous, current>
    using DataSourceUpdateEventListener = DataSourceEventListener<size_t, uint8_t, uint8_t>;

    using DataSourceFlagUpdatedHandler = std::function<void(bool, bool)>;
    using DataSourceByteUpdatedHandler = std::function<void(uint8_t, uint8_t)>;

#include "data_source_events.tpp"
}