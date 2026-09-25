#ifndef IDLE_STATE_H
#define IDLE_STATE_H

#include <core/state_machine/state/i_state.h>

class IdleState : public IState
{
public:
    explicit IdleState(IState* parent);

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

#endif // IDLE_STATE_H
