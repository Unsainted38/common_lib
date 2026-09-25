#include "stop_event.h"

StopEvent::StopEvent() {}


EventType StopEvent::type() const
{
    return EventType::Stop;
}

EventPriority StopEvent::priority() const
{
    return EventPriority::High;
}