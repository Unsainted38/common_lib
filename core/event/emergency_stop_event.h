#ifndef EMERGENCY_STOP_EVENT_H
#define EMERGENCY_STOP_EVENT_H

#include "i_event.h"

class EmergencyStopEvent : public IEvent
{
public:
    EmergencyStopEvent();

    // IEvent interface
public:
    EventType type() const override;
    EventPriority priority() const override;
};

#endif // EMERGENCY_STOP_EVENT_H
