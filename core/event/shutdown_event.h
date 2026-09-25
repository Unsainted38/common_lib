#ifndef SHUTDOWN_EVENT_H
#define SHUTDOWN_EVENT_H

#include "i_event.h"

class ShutdownEvent : public IEvent
{
public:
    ShutdownEvent();

    // IEvent interface
public:
    EventType type() const override;
    EventPriority priority() const override;
};

#endif // SHUTDOWN_EVENT_H
