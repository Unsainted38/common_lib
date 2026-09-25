#include "ready_state.h"

ReadyState::ReadyState(IState *parent) : parent_(parent) {}


StateId ReadyState::id() const
{
    return StateId::Ready;
}


IState *ReadyState::parent() const
{
    return parent_;
}

void ReadyState::on_enter()
{
}

void ReadyState::on_exit()
{
}

HandleResult ReadyState::handle(const IEvent &event)
{
    switch (event.type()) {
    default:
        return HandleResult::ignored();
    }
}


