#ifndef STOP_EVENT_H
#define STOP_EVENT_H

#include "i_event.h"

class StopEvent : public IEvent
{
public:
    StopEvent();

    // IEvent interface
public:
    EventType type() const override;
    EventPriority priority() const override;
};

#endif // STOP_EVENT_H
