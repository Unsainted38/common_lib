#ifndef START_AUTO_TRACKING_EVENT_H
#define START_AUTO_TRACKING_EVENT_H

#include "i_event.h"

class StartAutoTrackingEvent : public IEvent
{
public:
    StartAutoTrackingEvent();

    // IEvent interface
public:
    EventType type() const override;
    EventPriority priority() const override;
};

#endif // START_AUTO_TRACKING_EVENT_H
