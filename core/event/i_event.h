#ifndef I_EVENT_H
#define I_EVENT_H

#include <core/event/event_priority.h>
#include <core/event/event_type.h>

class IEvent {
public:
    virtual ~IEvent() = default;

    virtual EventType type() const = 0;
    virtual EventPriority priority() const = 0;
};

#endif // I_EVENT_H
