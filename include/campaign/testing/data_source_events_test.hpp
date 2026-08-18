#pragma once

#if CAMPAIGN_LIBRARY_TESTING

#include <campaign/data_source_events.hpp>
#include <cstdint>

namespace campaign::testing
{
    class DataSourceEventTest
    {
    public:
        DataSourceEventTest() = delete;

        template <typename... TArgs>
        static const std::unordered_map<DataSourceEventHandlerKey, std::function<void(TArgs...)>> &getListenerMap(const DataSourceEvent<TArgs...> &event)
        {
            return event.listenersMap_;
        }

        static const DataSourceUpdateEvent &getUpdateEvent(const DataSource &source)
        {
            return source.updateEvent_;
        }

        static const std::unordered_map<DataSourceEventHandlerKey, std::function<void(size_t, uint8_t, uint8_t)>> &getUpdateEventListenerMap(const DataSource &source)
        {
            return getListenerMap<size_t, uint8_t, uint8_t>(source.updateEvent_);
        }
    };
}

#endif // CAMPAIGN_LIBRARY_TESTING