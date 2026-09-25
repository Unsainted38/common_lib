#include "failure_event.h"

FailureEvent::FailureEvent() {}


EventType FailureEvent::type() const
{
    return EventType::Failure;
}

EventPriority FailureEvent::priority() const
{
    return EventPriority::Critical;
}