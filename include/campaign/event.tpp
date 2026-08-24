template <typename... TArgs>
void Event<TArgs...>::call(TArgs... args) const
{
    for (auto &l : listenersMap_)
    {
        l.second->call(args...);
    }
}

template <typename... TArgs>
EventListenerToken Event<TArgs...>::registerCallback(const std::function<void(TArgs...)> &handler)
{
    auto listenerPtr = std::make_shared<Listener>(*this, handler);

    listenersMap_.insert({listenerPtr->getKey(), listenerPtr});

    return EventListenerToken(listenerPtr);
}

template <typename... TArgs>
void Event<TArgs...>::unregisterCallback(EventListenerKey key)
{
    listenersMap_.erase(key);
}

template <typename... TArgs>
EventListener<TArgs...>::EventListener(IEvent &event, const std::function<void(TArgs...)> &callback)
    : IEventListener(event)
{
    callback_ = callback;
}

template <typename... TArgs>
void EventListener<TArgs...>::call(TArgs... args) const
{
    callback_(args...);
}

template <typename... TArgs>
EventListenerToken::EventListenerToken(const std::shared_ptr<EventListener<TArgs...>> listener)
{
    listener_ = static_cast<std::shared_ptr<EventListener<TArgs...>>>(listener);
}

