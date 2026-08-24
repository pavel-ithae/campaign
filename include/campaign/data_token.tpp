template <typename T>
DataDynamicToken<T>::DataDynamicToken(const std::weak_ptr<Proxy> &sourcePtr, size_t index)
    : DataToken(sourcePtr), index_(index)
{
}

template <typename T>
void DataDynamicToken<T>::set(const T &value) const
{
    getSource().set<T>(index_, value);
}

template <typename T>
inline const T &campaign::DataDynamicToken<T>::get() const
{
    return getSource().get<T>(index_);
}
