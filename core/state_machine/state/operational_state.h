#ifndef OPERATIONAL_STATE_H
#define OPERATIONAL_STATE_H

#include <core/state_machine/state/i_state.h>

class OperationalState : public IState
{
public:
    OperationalState();

    // IState interface
public:
    StateId id() const override;
    HandleResult handle(const IEvent &event) override;

    void on_enter() override;
    void on_exit() override;
};

#endif // OPERATIONAL_STATE_H
