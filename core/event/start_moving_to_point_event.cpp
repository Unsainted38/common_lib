#include "start_moving_to_point_event.h"

StartMovingToPointEvent::StartMovingToPointEvent(double azimuth, double elevation) :
    azimuth_(azimuth),
    elevation_(elevation) {

}

EventPriority StartMovingToPointEvent::priority() const
{
    return EventPriority::Normal;
}

EventType StartMovingToPointEvent::type() const
{
    return EventType::StartMovingToPoint;
}

double StartMovingToPointEvent::azimuth() const {
    return azimuth_;
}

double StartMovingToPointEvent::elevation() const {
    return elevation_;
}
