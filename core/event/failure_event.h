#ifndef FAILURE_EVENT_H
#define FAILURE_EVENT_H

#include "i_event.h"

class FailureEvent : public IEvent
{
public:
    FailureEvent();

    // IEvent interface
public:
    EventType type() const override;
    EventPriority priority() const override;
};

#endif // FAILURE_EVENT_H
