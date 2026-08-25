template <typename T>
ProxyDynamicToken<T>::ProxyDynamicToken(const std::weak_ptr<Proxy> &sourcePtr, size_t index)
    : ProxyToken(sourcePtr), index_(index)
{
}

template <typename T>
void ProxyDynamicToken<T>::set(const T &value) const
{
    getSource().set<T>(index_, value);
}

template <typename T>
inline T campaign::ProxyDynamicToken<T>::get() const
{
    return getSource().get<T>(index_);
}
