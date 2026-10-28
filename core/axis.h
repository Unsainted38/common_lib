#ifndef AXIS_H
#define AXIS_H

#include <core/driver_controller.h>
#include <sensors/encoders/i_position_sensor.h>
#include <sensors/limiters/i_limiters_sensor.h>
#include <memory>


struct WorkingSector {
    double minus = 0.0;
    double plus = 360.0;
};

struct AxisState {
    Measurement<PositionMeasurement> position;
    int32_t turns = 0;

    Measurement<LimitersState> limiters;

    bool positionValid = false;
    bool limitersValid = false;

    WorkingSector working_sector;
};


class Axis
{
    std::shared_ptr<DriverController> driver_controller_;
    std::shared_ptr<IPositionSensor> position_sensor_;
    std::shared_ptr<ILimitersSensor> limiters_sensor_;

    WorkingSector working_sector_;
public:
    explicit Axis(std::shared_ptr<DriverController> driver_controller, std::shared_ptr<IPositionSensor> position_sensor, std::shared_ptr<ILimitersSensor> limiters_sensor);

    AxisState state();
    void move(double velocity);
    void stop();
};

#endif // AXIS_H
