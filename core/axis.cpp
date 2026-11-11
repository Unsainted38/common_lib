#include "axis.h"

Axis::Axis(std::shared_ptr<DriverController> driver_controller,
           std::shared_ptr<IPositionSensor> position_sensor,
           std::shared_ptr<ILimitersSensor> limiters_sensor,
           std::unique_ptr<PID> pid) :
    driver_controller_(std::move(driver_controller)),
    position_sensor_(std::move(position_sensor)),
    limiters_sensor_(std::move(limiters_sensor)),
    pid_(std::move(pid)) {

}

AxisState Axis::state() const
{
    return {
        .position = position_sensor_->value(),
        .limiters = {limiters_sensor_->value().value},
        .positionValid = position_sensor_->value().valid,
        .limitersValid = limiters_sensor_->value().valid,
        .working_sector = working_sector_
    };
}

double Axis::calculate_velocity(double target, double precision, double dt)
{
    if (target - precision < state().position.value.angle && state().position.value.angle < target + precision) {
        return 0.0;
    }
    return pid_->calculateOP(state().position.value.angle, target, dt);
}

void Axis::reset_pid()
{
    pid_->reset_pid();
}

void Axis::move(double velocity)
{
    driver_controller_->move(velocity);
}

void Axis::stop()
{
    driver_controller_->stop();
}
