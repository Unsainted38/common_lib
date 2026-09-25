#ifndef MOVING_STATE_H
#define MOVING_STATE_H

#include "i_state.h"

class MovingState : public IState
{
public:
    explicit MovingState(IState* parent);

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

#endif // MOVING_STATE_H
