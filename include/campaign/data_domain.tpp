template <typename T>
DataDynamicToken<T> DataDomain::getDynamicToken(const std::string locationID, const std::string dynamicID)
{
    return locators_.at(locationID).getDynamicToken<T>(dynamicID);
}