#include "state_machine.h"
#include "state/auto_tracking_state.h"
#include "state/extreme_tracking_state.h"
#include "state/target_designation_tracking_state.h"
#include "state/error_state.h"
#include "state/moving_state.h"
#include "state/initializing_state.h"
#include "state/moving_to_point_state.h"
#include "state/idle_state.h"

StateMachine::StateMachine() :
    error_(std::make_unique<ErrorState>()),
    initializing_(std::make_unique<InitializingState>()),
    operational_(std::make_unique<OperationalState>()),
    idle_(std::make_unique<IdleState>(operational_.get())),
    extreme_(std::make_unique<ExtremeTrackingState>(operational_.get())),
    auto_(std::make_unique<AutoTrackingState>(operational_.get())),
    designation_(std::make_unique<TargetDesignationTrackingState>(operational_.get())),
    moving_(std::make_unique<MovingState>(operational_.get())),
    to_point_(std::make_unique<MovingToPointState>(operational_.get())),
    current_state_(operational_.get())
{
    current_state_->on_enter();
}

StateMachine::~StateMachine() = default;

void StateMachine::handle(const IEvent &event)
{
    IState* state = current_state_;

    while (state != nullptr) {
        const HandleResult result = state->handle(event);

        if (result.type() == HandleResult::Type::Ignored) {
            state = state->parent();
            continue;
        }

        if (result.type() == HandleResult::Type::Handled) {
            return;
        }

        if (result.type() == HandleResult::Type::Transition) {
            transition_to(result.target());
            return;
        }
    }
}

StateId StateMachine::current_state() const
{
    return current_state_->id();
}

IState *StateMachine::get_state(StateId id)
{
    switch (id) {
    case StateId::Idle:
        return idle_.get();
    case StateId::AutoTracking:
        return auto_.get();
    case StateId::ExtremeTracking:
        return extreme_.get();
    case StateId::TargetDesignationTracking:
        return designation_.get();
    case StateId::Error:
        return error_.get();
    case StateId::Initializing:
        return initializing_.get();
    case StateId::Moving:
        return moving_.get();
    case StateId::MovingToPoint:
        return to_point_.get();
    case StateId::Operational:
        return operational_.get();
    }
    return nullptr;
}

std::vector<IState *> StateMachine::build_path(IState *state) const
{
    std::vector<IState*> path;

    while (state != nullptr)
    {
        path.push_back(state);
        state = state->parent();
    }

    return path;
}

void StateMachine::transition_to(StateId target_id)
{
    IState* target = get_state(target_id);

    if (target == nullptr)
        return;

    if (target == current_state_)
        return;

    std::vector<IState*> currentPath =
        build_path(current_state_);

    std::vector<IState*> targetPath =
        build_path(target);

    std::size_t currentIndex =
        currentPath.size();

    std::size_t targetIndex =
        targetPath.size();

    // Ищем общего предка.
    while (
        currentIndex > 0 &&
        targetIndex > 0 &&
        currentPath[currentIndex - 1] ==
            targetPath[targetIndex - 1])
    {
        --currentIndex;
        --targetIndex;
    }

    // Выходим из текущей ветки снизу вверх.
    for (std::size_t i = 0;
         i < currentIndex;
         ++i)
    {
        currentPath[i]->on_exit();
    }

    // Входим в новую ветку сверху вниз.
    for (std::size_t i = targetIndex;
         i > 0;
         --i)
    {
        targetPath[i - 1]->on_enter();
    }

    current_state_ = target;
}
