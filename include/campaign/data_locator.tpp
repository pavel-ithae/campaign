template <typename T>
void DataLocator::set(std::string id, const T &value)
{
    auto desc = layout_.getDynamicDescription(id, sizeof(T));

    sourcePtr_.lock()->set<T>(desc.index, value);
}

template <typename T>
T DataLocator::get(std::string id)
{
    auto desc = layout_.getDynamicDescription(id, sizeof(T));

    return sourcePtr_.lock()->get<T>(desc.index);
}

template <typename T>
DataDynamicToken<T> DataLocator::getDynamicToken(std::string id)
{
    auto desc = layout_.getDynamicDescription(id, sizeof(T));

    return DataDynamicToken<T>(sourcePtr_, desc.index);
}