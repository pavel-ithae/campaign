#include <campaign/data_source_events.hpp>
#include <memory>
#include <iostream>

using namespace campaign;

IDataSourceEventListener::IDataSourceEventListener()
{
    key_ = new char;
}

IDataSourceEventListener::~IDataSourceEventListener()
{
    delete key_;
}

DataSourceEventHandlerKey IDataSourceEventListener::getKey() const
{
    return key_;
}

DataSourceEventListenerToken::DataSourceEventListenerToken(IDataSourceEvent &event, const std::shared_ptr<IDataSourceEventListener> &listener)
{
    event_ = &event;
    listener_ = listener;
}

void DataSourceEventListenerToken::unregister() const
{
    if (!isActive())
    {
        return;
    }

    auto listenerLocked = listener_.lock();

    event_->unregisterCallback(listenerLocked->getKey());
}

bool DataSourceEventListenerToken::isActive() const
{
    return !listener_.expired();
}