#ifndef EXTREME_TRACKING_STATE_H
#define EXTREME_TRACKING_STATE_H

#include "i_state.h"

class ExtremeTrackingState : public IState
{
public:
    explicit ExtremeTrackingState(IState* parent);

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

#endif // EXTREME_TRACKING_STATE_H
