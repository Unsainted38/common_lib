#include "pointing_mode.h"

PointingMode::PointingMode(std::vector<PointingAxis> axes, double precision) :
    axes_(std::move(axes)),
    precision_(precision) {
}

void PointingMode::reset()
{
    for (std::size_t i = 0; i < axes_.size(); ++i) {
        axes_[i].axis->reset_pid();
    }
}

ControlOutput PointingMode::calculate(const ControlInput &input)
{
    ControlOutput output;
    output.velocities.reserve(axes_.size());
    for (std::size_t i = 0; i < axes_.size(); ++i) {
        const auto& [axis, target] = axes_[i];
        double velocity = axis->calculate_velocity(
            target,
            precision_,
            input.dt);
        output.velocities.push_back(velocity);
    }
    return output;
}

bool PointingMode::requires_signal()
{
    return false;
}
