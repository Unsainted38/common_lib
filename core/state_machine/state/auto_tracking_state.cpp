#include "auto_tracking_state.h"

AutoTrackingState::AutoTrackingState(IState* parent) : parent_(parent) {}


StateId AutoTrackingState::id() const
{
    return StateId::AutoTracking;
}

IState *AutoTrackingState::parent() const
{
    return parent_;
}

void AutoTrackingState::on_enter()
{
}

void AutoTrackingState::on_exit()
{
}

HandleResult AutoTrackingState::handle(const IEvent &event)
{
    switch (event.type()) {
    default:
        return HandleResult::ignored();
    }
}
