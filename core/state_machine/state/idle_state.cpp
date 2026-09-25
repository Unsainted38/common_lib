#include "idle_state.h"
#include <iostream>

IdleState::IdleState(IState *parent) : parent_(parent)
{

}

StateId IdleState::id() const
{
    return StateId::Idle;
}

IState *IdleState::parent() const
{
    return parent_;
}

void IdleState::on_enter()
{
    std::cout << "enter Idle\n";
}

void IdleState::on_exit()
{
    std::cout << "exit Idle\n";
}

HandleResult IdleState::handle(const IEvent &event)
{
    switch (event.type()) {
    default:
        return HandleResult::ignored();
        break;
    }
}
