#include "moving_to_point_state.h"
#include <core/event/start_moving_to_point_event.h>

MovingToPointState::MovingToPointState(IState *parent) : parent_(parent) {

}


StateId MovingToPointState::id() const
{
    return StateId::MovingToPoint;
}

IState *MovingToPointState::parent() const {
    return parent_;
}

void MovingToPointState::on_enter()
{

    std::cout << "enter MovingToPointState\n";
}

void MovingToPointState::on_exit()
{
    std::cout << "exit MovingToPointState\n";
}

HandleResult MovingToPointState::handle(const IEvent &event)
{
    switch (event.type()) {
    case EventType::StartMovingToPoint: {
        const auto& current_event = dynamic_cast<const StartMovingToPointEvent&>(event);
        return HandleResult::handled();
    }
    default:
        return HandleResult::ignored();
    }
}
