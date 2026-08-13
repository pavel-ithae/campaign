#pragma once

#include <campaign/data_source_events.hpp>
#include <cstdint>
#include <vector>

namespace campaign
{
    class DataSource
    {
    private:
        std::vector<uint8_t> data_;

        DataSourceUpdateEvent updateEvent_;

        void setBlock(size_t index, void *ptr, size_t range);

        void notifyByteUpdated(size_t index, uint8_t previous, uint8_t current) const;

        void *getBlock(size_t index, size_t range);

    public:
        DataSource(int size);
        ~DataSource();

        void init();
        void init(uint8_t *copyPtr);

        bool setFlag(size_t index, uint8_t flagMask, bool value);
        bool setByte(size_t index, uint8_t value);

        template <typename T>
        void set(size_t index, const T &value)
        {
            setBlock(index, &value, sizeof(T));
        }

        DataSourceEventListenerToken registerFlagCallback(size_t index, uint8_t flagMask, const DataSourceFlagUpdatedHandler &handler);
        DataSourceEventListenerToken registerByteCallback(size_t index, const DataSourceByteUpdatedHandler &handler);

        void unregisterUpdateCallback(DataSourceEventHandlerKey key);

        bool getFlag(size_t index, uint8_t flagMask) const;
        uint8_t getByte(size_t index) const;

        template <typename T>
        T get(size_t index)
        {
            return *static_cast<T *>(getBlock(index, sizeof(T)));
        }

        std::size_t getSize() const;
    };
}