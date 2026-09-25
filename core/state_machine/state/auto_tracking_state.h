#ifndef AUTO_TRACKING_STATE_H
#define AUTO_TRACKING_STATE_H

#include "i_state.h"

class AutoTrackingState : public IState
{
public:
    explicit AutoTrackingState(IState* parent);

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

#endif // AUTO_TRACKING_STATE_H
