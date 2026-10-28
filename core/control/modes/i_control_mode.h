#ifndef I_CONTROL_MODE_H
#define I_CONTROL_MODE_H

#include <core/event/i_event.h>
#include <core/axis.h>

struct ControlOutput {
    double azVelocity = 0.0;
    double elVelocity = 0.0;
};

struct ControlInput {
    AxisState az;
    AxisState el;
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
