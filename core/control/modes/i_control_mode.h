#ifndef I_CONTROL_MODE_H
#define I_CONTROL_MODE_H

#include <core/event/i_event.h>
#include <core/axis.h>

struct ControlOutput {
    std::vector<double> velocities;
};

struct ControlInput {
    std::vector<AxisState> axes;
    Measurement<double> signal_power;
    double dt = 0.0;
};

struct IControlMode {
    virtual ~IControlMode() = default;
    virtual void reset() = 0;
    virtual ControlOutput calculate(const ControlInput& input) = 0;
    virtual bool requires_signal() = 0;
};

#endif // I_CONTROL_MODE_H
