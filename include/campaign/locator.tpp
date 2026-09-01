template <typename T>
void Locator::set(const std::string &id, const T &value)
{
    auto info = layout_.getDynamicInfo(id);

    validateDynamicSize(sizeof(T), info.size);

    proxyPtr_.lock()->set<T>(info.index, value);
}

template <typename T>
T Locator::get(const std::string &id)
{
    auto info = layout_.getDynamicInfo(id);

    validateDynamicSize(sizeof(T), info.size);

    return proxyPtr_.lock()->get<T>(info.index);
}

template <typename T>
ProxyDynamicToken<T> Locator::getDynamicToken(const std::string &id)
{
    auto info = layout_.getDynamicInfo(id);

    validateDynamicSize(sizeof(T), info.size);

    return ProxyDynamicToken<T>(proxyPtr_, info.index);
}