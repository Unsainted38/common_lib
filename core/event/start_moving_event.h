#ifndef START_MOVING_EVENT_H
#define START_MOVING_EVENT_H

#include "i_event.h"

class StartMovingEvent : public IEvent
{
public:
    StartMovingEvent();

    // IEvent interface
public:
    EventType type() const override;
    EventPriority priority() const override;
};

#endif // START_MOVING_EVENT_H
