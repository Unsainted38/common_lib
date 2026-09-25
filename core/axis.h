#ifndef AXIS_H
#define AXIS_H

#include <core/driver_controller.h>
#include <encoders/i_encoder.h>
#include <memory>

class Axis
{
    std::unique_ptr<DriverController> driver_controller;
    std::unique_ptr<IEncoder> encoder;
public:
    explicit Axis();
};

#endif // AXIS_H
