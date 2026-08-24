#include <campaign/event.hpp>

using namespace campaign;

IEventListener::IEventListener(IEvent &event)
    : event_(&event), key_(new char)
{
}

IEventListener::~IEventListener()
{
    delete key_;
}

void IEventListener::unregister()
{
    if (event_ == nullptr)
    {
        return;
    }

    event_->unregisterCallback(key_);
    event_ = nullptr;
}

EventListenerKey IEventListener::getKey() const
{
    return key_;
}

EventListenerToken::EventListenerToken(const std::shared_ptr<IEventListener> &listener)
{
    listener_ = listener;
}

void EventListenerToken::unregister() const
{
    if (!isActive())
    {
        return;
    }

    auto listenerLocked = listener_.lock();

    listenerLocked->unregister();
}

bool EventListenerToken::isActive() const
{
    return !listener_.expired();
}