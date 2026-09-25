#ifndef INIT_COMPLETED_EVENT_H
#define INIT_COMPLETED_EVENT_H

#include "i_event.h"

class InitCompletedEvent : public IEvent
{
public:
    InitCompletedEvent();

    // IEvent interface
public:
    EventType type() const override;
    EventPriority priority() const override;
};

#endif // INIT_COMPLETED_EVENT_H
