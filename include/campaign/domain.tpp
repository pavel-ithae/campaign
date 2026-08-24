template <typename T>
inline void Domain::set(const std::string &locationID, const std::string &dynamicID, const T &value)
{
    locators_.at(locationID).set<T>(dynamicID, value);
}

template <typename T>
inline T campaign::Domain::get(const std::string &locationID, const std::string &dynamicID)
{
    return locators_.at(locationID).get<T>(dynamicID);
}

template <typename T>
ProxyDynamicToken<T> Domain::getDynamicToken(const std::string locationID, const std::string dynamicID)
{
    return locators_.at(locationID).getDynamicToken<T>(dynamicID);
}