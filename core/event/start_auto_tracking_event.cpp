#include "start_auto_tracking_event.h"

StartAutoTrackingEvent::StartAutoTrackingEvent() {}


EventType StartAutoTrackingEvent::type() const
{
    return EventType::StartAutoTracking;
}

EventPriority StartAutoTrackingEvent::priority() const
{
    return EventPriority::Normal;
}