#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>

namespace campaign
{
    class DataSource;
    class IDataSourceEvent;

    using DataSourceEventHandlerKey = char *;

    /// @brief Tracks state of a registered event callback.
    class DataSourceEventListenerState
    {
    public:
        DataSourceEventListenerState(DataSourceEventHandlerKey key, bool active)
        {
            key_ = key;
            active_ = active;
        }

        void release();

        inline DataSourceEventHandlerKey getKey() { return key_; }
        inline bool isActive() { return active_; }

    private:
        DataSourceEventHandlerKey key_;
        bool active_;
    };

    /// @brief Token to keep track of a registered callback.
    class DataSourceEventListenerToken
    {
        // TODO: Implement a way to automatically release the token when data source is removed.
    public:
        DataSourceEventListenerToken(IDataSourceEvent &event, DataSourceEventHandlerKey key);

        void unregister() const;

        bool isActive() const;

    private:
        IDataSourceEvent *event_;
        std::shared_ptr<DataSourceEventListenerState> state_;
    };

    /// @brief Base class for data source events.
    class IDataSourceEvent
    {
    public:
        virtual void unregisterCallback(DataSourceEventHandlerKey key) = 0;
    };

    /// @brief Template for data source events.
    /// @tparam ...TArgs
    template <typename... TArgs>
    class DataSourceEvent : IDataSourceEvent
    {
    public:
        void call(TArgs... args) const;

        DataSourceEventListenerToken registerCallback(const std::function<void(TArgs...)> &handler);

        void unregisterCallback(DataSourceEventHandlerKey key) override;

    private:
        std::unordered_map<DataSourceEventHandlerKey, std::function<void(TArgs...)>> listenersMap_;
    };

    using DataSourceUpdateEvent = DataSourceEvent<size_t, uint8_t, uint8_t>; // <index, previous, current>

    using DataSourceFlagUpdatedHandler = std::function<void(bool, bool)>;
    using DataSourceByteUpdatedHandler = std::function<void(uint8_t, uint8_t)>;

#include "data_source_events.tpp"
}