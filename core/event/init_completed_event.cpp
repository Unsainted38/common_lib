#include "init_completed_event.h"

InitCompletedEvent::InitCompletedEvent() {}


EventType InitCompletedEvent::type() const
{
    return EventType::InitCompleted;
}

EventPriority InitCompletedEvent::priority() const
{
    return EventPriority::High;
}