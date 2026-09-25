#include "initializing_state.h"

InitializingState::InitializingState() {}


StateId InitializingState::id() const
{
    return StateId::Initializing;
}

void InitializingState::on_enter()
{
}

void InitializingState::on_exit()
{
}

HandleResult InitializingState::handle(const IEvent &event)
{
    switch (event.type()) {
    case EventType::Failure:
        return HandleResult::transitionTo(StateId::Error);
    case EventType::Reset:
        return HandleResult::transitionTo(StateId::Initializing);
    case EventType::InitCompleted:
        return HandleResult::transitionTo(StateId::Idle);
    default:
        return HandleResult::ignored();
    }
}
