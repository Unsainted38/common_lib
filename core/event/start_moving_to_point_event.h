#ifndef START_MOVING_TO_POINT_EVENT_H
#define START_MOVING_TO_POINT_EVENT_H

#include <core/event/i_event.h>

class StartMovingToPointEvent : public IEvent
{
public:
    explicit StartMovingToPointEvent(double azimuth, double elevation);
    EventPriority priority() const override;
    EventType type() const override;
    double azimuth() const;
    double elevation() const;
private:
    double azimuth_;
    double elevation_;
};

#endif // START_MOVING_TO_POINT_EVENT_H
