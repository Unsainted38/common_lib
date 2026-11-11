#ifndef AXIS_H
#define AXIS_H

#include <core/driver_controller.h>
#include <sensors/encoders/i_position_sensor.h>
#include <sensors/limiters/i_limiters_sensor.h>
#include <utilities/pid.h>
#include <memory>


struct WorkingSector {
    double minus = 0.0;
    double plus = 360.0;
};

struct AxisState {
    Measurement<PositionMeasurement> position;

    Measurement<LimitersState> limiters;

    bool positionValid = false;
    bool limitersValid = false;

    WorkingSector working_sector;
};


class Axis
{
private:
    std::shared_ptr<DriverController> driver_controller_;
    std::shared_ptr<IPositionSensor> position_sensor_;
    std::shared_ptr<ILimitersSensor> limiters_sensor_;
    std::unique_ptr<PID> pid_;

    WorkingSector working_sector_;
public:
    explicit Axis(std::shared_ptr<DriverController> driver_controller,
                  std::shared_ptr<IPositionSensor> position_sensor,
                  std::shared_ptr<ILimitersSensor> limiters_sensor,
                  std::unique_ptr<PID> pid);

    AxisState state() const;
    double calculate_velocity(double target, double precision, double dt);
    void reset_pid();
    void move(double velocity);
    void stop();
};

#endif // AXIS_H
