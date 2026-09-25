#include "operational_state.h"
#include <iostream>

OperationalState::OperationalState() {}


StateId OperationalState::id() const
{
    return StateId::Operational;
}

HandleResult OperationalState::handle(const IEvent &event)
{
    switch (event.type()) {
    case EventType::EmergencyStop:
        return HandleResult::transitionTo(StateId::Error);

    case EventType::Failure:
        return HandleResult::transitionTo(StateId::Error);

    case EventType::StartMovingToPoint:
        return HandleResult::transitionTo(StateId::MovingToPoint);

    case EventType::StartMoving:
        return HandleResult::transitionTo(StateId::Moving);

    case EventType::StartAutoTracking:
        return HandleResult::transitionTo(StateId::AutoTracking);

    case EventType::StartExtremeTracking:
        return HandleResult::transitionTo(StateId::ExtremeTracking);

    case EventType::InitCompleted:
        return HandleResult::transitionTo(StateId::Idle);

    case EventType::Reset:
        return HandleResult::transitionTo(StateId::Initializing);

    case EventType::PowerOn:
        return HandleResult::transitionTo(StateId::Initializing);

    case EventType::StartTargetDesignationTracking:
        return HandleResult::transitionTo(StateId::TargetDesignationTracking);

    case EventType::Stop:
        return HandleResult::transitionTo(StateId::Idle);

    default:
        return HandleResult::ignored();

    }
}


void OperationalState::on_enter()
{
    std::cout << "enter Operational\n";
}

void OperationalState::on_exit()
{
    std::cout << "exit Operational";
}
