template <typename... TArgs>
DataSourceEventListener<TArgs...>::DataSourceEventListener(const std::function<void(TArgs...)> &callback)
    : IDataSourceEventListener()
{
    callback_ = callback;
}

template <typename... TArgs>
void DataSourceEventListener<TArgs...>::call(TArgs... args) const
{
    callback_(args...);
}

template <typename... TArgs>
void DataSourceEvent<TArgs...>::call(TArgs... args) const
{
    for (auto &l : listenersMap_)
    {
        l.second->call(args...);
    }
}

template <typename... TArgs>
DataSourceEventListenerToken::DataSourceEventListenerToken(IDataSourceEvent &event, const std::shared_ptr<DataSourceEventListener<TArgs...>> listener)
{
    event_ = &event;
    listener_ = static_cast<std::shared_ptr<DataSourceEventListener<TArgs...>>>(listener);
}

template <typename... TArgs>
DataSourceEventListenerToken DataSourceEvent<TArgs...>::registerCallback(const std::function<void(TArgs...)> &handler)
{
    auto listenerPtr = std::make_shared<Listener>(handler);

    listenersMap_.insert({listenerPtr->getKey(), listenerPtr});

    return DataSourceEventListenerToken(*static_cast<IDataSourceEvent *>(this), listenerPtr);
}

template <typename... TArgs>
void DataSourceEvent<TArgs...>::unregisterCallback(DataSourceEventHandlerKey key)
{
    listenersMap_.erase(key);
}