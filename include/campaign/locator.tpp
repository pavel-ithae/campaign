template <typename T>
void Locator::set(std::string id, const T &value)
{
    auto info = layout_.getDynamicInfo(id, sizeof(T));

    proxyPtr_.lock()->set<T>(info.index, value);
}

template <typename T>
T Locator::get(std::string id)
{
    auto info = layout_.getDynamicInfo(id, sizeof(T));

    return proxyPtr_.lock()->get<T>(info.index);
}

template <typename T>
ProxyDynamicToken<T> Locator::getDynamicToken(std::string id)
{
    auto info = layout_.getDynamicInfo(id, sizeof(T));

    return ProxyDynamicToken<T>(proxyPtr_, info.index);
}