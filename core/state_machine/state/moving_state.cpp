#include "moving_state.h"

MovingState::MovingState(IState *parent) : parent_(parent)
{

}

StateId MovingState::id() const
{
    return StateId::Moving;
}

IState *MovingState::parent() const
{
    return parent_;
}

void MovingState::on_enter()
{
}

void MovingState::on_exit()
{
}

HandleResult MovingState::handle(const IEvent &event)
{
    switch (event.type()) {
    default:
        return HandleResult::ignored();
    }
}
