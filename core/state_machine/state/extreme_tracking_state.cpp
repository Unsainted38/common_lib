#include "extreme_tracking_state.h"


ExtremeTrackingState::ExtremeTrackingState(IState *parent) : parent_(parent)
{

}

StateId ExtremeTrackingState::id() const
{
    return StateId::ExtremeTracking;
}

IState *ExtremeTrackingState::parent() const
{
    return parent_;
}

void ExtremeTrackingState::on_enter()
{
}

void ExtremeTrackingState::on_exit()
{
}

HandleResult ExtremeTrackingState::handle(const IEvent &event)
{
    switch (event.type()) {
    default:
        return HandleResult::ignored();
    }
}
