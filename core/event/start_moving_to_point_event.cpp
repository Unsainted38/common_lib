#include "start_moving_to_point_event.h"

StartMovingToPointEvent::StartMovingToPointEvent(const TargetPoint &target) :
    target_(target) {

}

EventPriority StartMovingToPointEvent::priority() const
{
    return EventPriority::Normal;
}

EventType StartMovingToPointEvent::type() const
{
    return EventType::StartMovingToPoint;
}
