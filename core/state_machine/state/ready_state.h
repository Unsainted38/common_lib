#ifndef READY_STATE_H
#define READY_STATE_H

#include <core/state_machine/state/i_state.h>

class ReadyState : public IState
{
public:
    explicit ReadyState(IState* parent);

    // IState interface
public:
    StateId id() const override;
    void on_enter() override;
    void on_exit() override;
    HandleResult handle(const IEvent &event) override;
    IState *parent() const override;
private:
    IState* parent_;
};

#endif // READY_STATE_H
