#include "error_state.h"

ErrorState::ErrorState() {}


StateId ErrorState::id() const
{
    return StateId::Error;
}

void ErrorState::on_enter()
{
}

void ErrorState::on_exit()
{
}

HandleResult ErrorState::handle(const IEvent &event)
{
    switch (event.type()) {
    case EventType::Reset:
        return HandleResult::transitionTo(StateId::Initializing);
    default:
        return HandleResult::ignored();
    }
}
