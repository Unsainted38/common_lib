#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <vector>
#include <memory>
#include "state/i_state.h"

class IdleState;
class OperationalState;
class ExtremeTrackingState;
class AutoTrackingState;
class TargetDesignationTrackingState;
class MovingState;
class MovingToPointState;
class ErrorState;
class InitializingState;

class StateMachine
{

public:
    explicit StateMachine();
    ~StateMachine();
    void handle(const IEvent& event);
    StateId current_state() const;

private:
    IState* get_state(StateId id);

    std::vector<IState*> build_path(IState* state) const;

    void transition_to(StateId target);


    std::unique_ptr<ErrorState> error_;
    std::unique_ptr<InitializingState> initializing_;
    std::unique_ptr<OperationalState> operational_;
    std::unique_ptr<IdleState> idle_;
    std::unique_ptr<ExtremeTrackingState> extreme_;
    std::unique_ptr<AutoTrackingState> auto_;
    std::unique_ptr<TargetDesignationTrackingState> designation_;
    std::unique_ptr<MovingState> moving_;
    std::unique_ptr<MovingToPointState> to_point_;

    IState *current_state_ = nullptr;

};

#endif // STATE_MACHINE_H
