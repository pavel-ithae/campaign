#pragma once

#include <campaign/data_source.hpp>
#include <campaign/data_layout.hpp>
#include <campaign/data_token.hpp>
#include <string>
#include <cinttypes>
#include <unordered_map>

namespace campaign
{
    class DataLocator
    {
    public:
        DataLocator(const DataLayout& layout, const std::weak_ptr<DataSource> &sourcePtr);

        // TODO: Add register callback functions and have tokens call these instead.

        void setFlag(std::string id, bool value);
        void setByte(std::string id, uint8_t value);

        template <typename T>
        void set(std::string id, const T &value);

        bool getFlag(std::string id);
        uint8_t getByte(std::string id);

        template <typename T>
        T get(std::string id);

        DataFlagToken getFlagToken(std::string id);
        DataByteToken getByteToken(std::string id);

        template <typename T>
        DataDynamicToken<T> getDynamicToken(std::string id);

    private:
        const DataLayout layout_;
        const std::weak_ptr<DataSource> sourcePtr_;
    };

    #include "data_locator.tpp"
}