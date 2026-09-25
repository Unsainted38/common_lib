#ifndef START_TARGET_DESIGNATION_TRACKING_EVENT_H
#define START_TARGET_DESIGNATION_TRACKING_EVENT_H

#include "i_event.h"

class StartTargetDesignationTrackingEvent : public IEvent
{
public:
    StartTargetDesignationTrackingEvent();

    // IEvent interface
public:
    EventType type() const override;
    EventPriority priority() const override;
};

#endif // START_TARGET_DESIGNATION_TRACKING_EVENT_H
