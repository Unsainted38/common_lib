#ifndef I_STATE_H
#define I_STATE_H

#include <core/state_machine/state/state_id.h>
#include <core/state_machine/state/handle_result.h>
#include <core/event/i_event.h>

#include <iostream>

class IState {
public:
    virtual ~IState() = default;

    virtual StateId id() const = 0;

    virtual IState* parent() const {
        return nullptr;
    }

    virtual void on_enter() {std::cout << "enter IState\n";}
    virtual void on_exit() {std::cout << "exit IState\n";}
    virtual HandleResult handle(const IEvent& event) = 0;
};

#endif // I_STATE_H
