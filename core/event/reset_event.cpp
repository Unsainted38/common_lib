#include "reset_event.h"

ResetEvent::ResetEvent() {}


EventType ResetEvent::type() const
{
    return EventType::Reset;
}

EventPriority ResetEvent::priority() const
{
    return EventPriority::High;
}