#include "emergency_stop_event.h"

EmergencyStopEvent::EmergencyStopEvent() {}


EventType EmergencyStopEvent::type() const
{
    return EventType::EmergencyStop;
}

EventPriority EmergencyStopEvent::priority() const
{
    return EventPriority::Critical;
}