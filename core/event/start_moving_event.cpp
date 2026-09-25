#include "start_moving_event.h"

StartMovingEvent::StartMovingEvent() {}


EventType StartMovingEvent::type() const
{
    return EventType::StartMoving;
}

EventPriority StartMovingEvent::priority() const
{
    return EventPriority::Normal;
}