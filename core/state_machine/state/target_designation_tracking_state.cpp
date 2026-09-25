#include "target_designation_tracking_state.h"

TargetDesignationTrackingState::TargetDesignationTrackingState(IState *parent) : parent_(parent) {}


StateId TargetDesignationTrackingState::id() const
{
    return StateId::TargetDesignationTracking;
}

IState *TargetDesignationTrackingState::parent() const
{
    return parent_;
}

void TargetDesignationTrackingState::on_enter()
{
}

void TargetDesignationTrackingState::on_exit()
{
}

HandleResult TargetDesignationTrackingState::handle(const IEvent &event)
{
    switch (event.type()) {
    default:
        return HandleResult::ignored();
    }
}
