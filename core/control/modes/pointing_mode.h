#ifndef POINTING_MODE_H
#define POINTING_MODE_H

#include "i_control_mode.h"
#include <core/axis.h>

struct PointingAxis {
    Axis* axis;
    double target;
};

class PointingMode : public IControlMode
{
private:
    std::vector<PointingAxis> axes_;
    double precision_ = 0.0;
public:
    explicit PointingMode(std::vector<PointingAxis> axes, double precision);

    // IControlMode interface
public:
    void reset() override;
    ControlOutput calculate(const ControlInput &input) override;
    bool requires_signal() override;
};

#endif // POINTING_MODE_H
