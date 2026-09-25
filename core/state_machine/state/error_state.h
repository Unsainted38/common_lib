#ifndef ERROR_STATE_H
#define ERROR_STATE_H

#include <core/state_machine/state/i_state.h>

class ErrorState : public IState
{
public:
    ErrorState();

    // IState interface
public:
    StateId id() const override;
    void on_enter() override;
    void on_exit() override;
    HandleResult handle(const IEvent &event) override;
};

#endif // ERROR_STATE_H
