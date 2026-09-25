#include "start_extreme_tracking_event.h"

StartExtremeTrackingEvent::StartExtremeTrackingEvent() {}


EventType StartExtremeTrackingEvent::type() const
{
    return EventType::StartExtremeTracking;
}

EventPriority StartExtremeTrackingEvent::priority() const
{
    return EventPriority::Normal;
}