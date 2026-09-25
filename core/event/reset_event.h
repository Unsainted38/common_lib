#ifndef RESET_EVENT_H
#define RESET_EVENT_H

#include "i_event.h"

class ResetEvent : public IEvent
{
public:
    ResetEvent();

    // IEvent interface
public:
    EventType type() const override;
    EventPriority priority() const override;
};

#endif // RESET_EVENT_H
