#ifndef TARGET_DESIGNATION_TRACKING_STATE_H
#define TARGET_DESIGNATION_TRACKING_STATE_H

#include "i_state.h"

class TargetDesignationTrackingState : public IState
{
public:
    explicit TargetDesignationTrackingState(IState* parent);

    // IState interface
public:
    StateId id() const override;
    IState *parent() const override;
    void on_enter() override;
    void on_exit() override;
    HandleResult handle(const IEvent &event) override;
private:
    IState* parent_;
};

#endif // TARGET_DESIGNATION_TRACKING_STATE_H
