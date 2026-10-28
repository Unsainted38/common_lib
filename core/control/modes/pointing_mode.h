#ifndef POINTING_MODE_H
#define POINTING_MODE_H

#include "i_control_mode.h"

class PointingMode : public IControlMode
{
public:
    PointingMode();

    // IControlMode interface
public:
    void reset() override;
    ControlOutput calculate(const ControlInput &input) override;
};

#endif // POINTING_MODE_H
