#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

namespace campaign
{
    // == Forward declarations. ==
    template <typename... TArgs>
    class EventListener;
    class EventListenerToken;

#if CAMPAIGN_LIBRARY_TESTING
    namespace testing
    {
        class EventTestHelper;
    }
#endif

    // == Event. ==
    using EventListenerKey = char *;

    class IEvent
    {
    protected:
        IEvent() {}

    public:
        virtual void unregisterCallback(EventListenerKey key) = 0;
    };

    template <typename... TArgs>
    class Event : public IEvent
    {
    public:
        Event() : IEvent() {}

        using Listener = EventListener<TArgs...>;

        void call(TArgs... args) const;

        EventListenerToken registerCallback(const std::function<void(TArgs...)> &handler);

        void unregisterCallback(EventListenerKey key) override;

    private:
        std::unordered_map<EventListenerKey, std::shared_ptr<Listener>> listenersMap_;

#if CAMPAIGN_LIBRARY_TESTING
        friend class testing::EventTestHelper;
#endif // CAMPAIGN_LIBRARY_TESTING
    };

    // == Listeners. ==
    class IEventListener
    {
    protected:
        IEventListener(IEvent &event);

    public:
        IEventListener(const IEventListener &) = delete; // Copy constructor could lead to deleting the key early and double deleting after.
        virtual ~IEventListener();

        IEventListener &operator=(const IEventListener &) = delete; // Copy assignment could lead to deleting the key early  and double deleting after.

        void unregister();

        EventListenerKey getKey() const;

    private:
        IEvent *event_;
        EventListenerKey const key_;
    };

    template <typename... TArgs>
    class EventListener : public IEventListener
    {
    public:
        EventListener(IEvent &event, const std::function<void(TArgs...)> &callback);
        EventListener(const EventListener<TArgs...> &) = delete; // Copy constructor could lead to deleting the key early and double deleting after.

        EventListener<TArgs...> &operator=(const EventListener<TArgs...> &) = delete; // Copy assignment could lead to deleting the key early  and double deleting after.

        void call(TArgs... args) const;

    private:
        std::function<void(TArgs...)> callback_;
    };

    // == Token. ==
    class EventListenerToken
    {
    public:
        EventListenerToken(const std::shared_ptr<IEventListener> &listener);

        template <typename... TArgs>
        EventListenerToken(const std::shared_ptr<EventListener<TArgs...>> listener);

        void unregister() const;

        bool isActive() const;

    private:
        std::weak_ptr<IEventListener> listener_;
    };

#include "event.tpp"
}