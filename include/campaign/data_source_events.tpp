template <typename... TArgs>
void DataSourceEvent<TArgs...>::call(TArgs... args) const
{
    for (auto &l : listenersMap_)
    {
        l.second(args...);
    }
}

template <typename... TArgs>
DataSourceEventListenerToken DataSourceEvent<TArgs...>::registerCallback(const std::function<void(TArgs...)> &handler)
{
    DataSourceEventHandlerKey key = new char;

    listenersMap_.insert({key, handler});

    return DataSourceEventListenerToken(*static_cast<IDataSourceEvent *>(this), key);
}

template <typename... TArgs>
void DataSourceEvent<TArgs...>::unregisterCallback(DataSourceEventHandlerKey key)
{
    listenersMap_.erase(key);
}