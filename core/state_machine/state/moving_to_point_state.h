#ifndef MOVING_TO_POINT_STATE_H
#define MOVING_TO_POINT_STATE_H

#include <core/state_machine/state/i_state.h>
#include <core/state_machine/state/operational_state.h>



class MovingToPointState : public IState
{
public:
    explicit MovingToPointState(IState *parent);

    // IState interface
public:
    StateId id() const override;
    IState* parent() const override;

    void on_enter() override;
    void on_exit() override;

    HandleResult handle(const IEvent &event) override;

private:
    IState* parent_;
    struct Target {
        double azimuth{};
        double elevation{};
    } target_;
};

#endif // MOVING_TO_POINT_STATE_H
