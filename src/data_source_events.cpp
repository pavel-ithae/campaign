#include <campaign/data_source_events.hpp>
#include <memory>

using namespace campaign;

void DataSourceEventListenerState::release()
{
    if (!active_)
    {
        return;
    }

    active_ = false;
    delete key_;
}

DataSourceEventListenerToken::DataSourceEventListenerToken(IDataSourceEvent &event, DataSourceEventHandlerKey key)
{
    event_ = &event;
    state_ = std::make_shared<DataSourceEventListenerState>(key, true);
}

void DataSourceEventListenerToken::unregister() const
{
    if (!state_->isActive())
    {
        return;
    }

    event_->unregisterCallback(state_->getKey());

    state_->release();
}

bool DataSourceEventListenerToken::isActive() const
{
    return state_->isActive();
}
