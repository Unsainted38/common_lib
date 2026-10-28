#ifndef START_MOVING_TO_POINT_EVENT_H
#define START_MOVING_TO_POINT_EVENT_H

#include <core/event/i_event.h>

struct TargetPoint {
    double azimuth{};
    double elevation{};
};

class StartMovingToPointEvent : public IEvent
{
public:
    explicit StartMovingToPointEvent(const TargetPoint& target);
    EventPriority priority() const override;
    EventType type() const override;
    const TargetPoint target() const;
private:
    TargetPoint target_;
};

#endif // START_MOVING_TO_POINT_EVENT_H
