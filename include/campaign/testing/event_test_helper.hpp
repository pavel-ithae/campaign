#pragma once

#if CAMPAIGN_LIBRARY_TESTING

#include <campaign/data_source.hpp>
#include <cstdint>

namespace campaign::testing
{
    class EventTestHelper
    {
    public:
        EventTestHelper() = delete;

        template <typename... TArgs>
        static const std::unordered_map<EventListenerKey, std::shared_ptr<typename Event<TArgs...>::Listener>> &getListenerMap(const Event<TArgs...> &event)
        {
            return event.listenersMap_;
        }

        static const DataSourceUpdateEvent &getUpdateEvent(const DataSource &source)
        {
            return source.updateEvent_;
        }

        static const std::unordered_map<EventListenerKey, std::shared_ptr<DataSourceUpdateEventListener>> &getUpdateEventListenerMap(const DataSource &source)
        {
            return getListenerMap<size_t, uint8_t, uint8_t>(source.updateEvent_);
        }
    };
}

#endif // CAMPAIGN_LIBRARY_TESTING