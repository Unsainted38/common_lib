#ifndef START_EXTREME_TRACKING_EVENT_H
#define START_EXTREME_TRACKING_EVENT_H

#include "i_event.h"

class StartExtremeTrackingEvent : public IEvent
{
public:
    StartExtremeTrackingEvent();

    // IEvent interface
public:
    EventType type() const override;
    EventPriority priority() const override;
};

#endif // START_EXTREME_TRACKING_EVENT_H
