#include "shutdown_event.h"

ShutdownEvent::ShutdownEvent() {}


EventType ShutdownEvent::type() const
{
    return EventType::Shutdown;
}

EventPriority ShutdownEvent::priority() const
{
    return EventPriority::Normal;
}
