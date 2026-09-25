#ifndef INITIALIZING_STATE_H
#define INITIALIZING_STATE_H

#include <core/state_machine/state/i_state.h>

class InitializingState : public IState
{
public:
    InitializingState();

    // IState interface
public:
    StateId id() const override;
    void on_enter() override;
    void on_exit() override;
    HandleResult handle(const IEvent &event) override;
};

#endif // INITIALIZING_STATE_H
