#include "start_target_designation_tracking_event.h"

StartTargetDesignationTrackingEvent::StartTargetDesignationTrackingEvent() {}


EventType StartTargetDesignationTrackingEvent::type() const
{
    return EventType::StartTargetDesignationTracking;
}

EventPriority StartTargetDesignationTrackingEvent::priority() const
{
    return EventPriority::Normal;
}