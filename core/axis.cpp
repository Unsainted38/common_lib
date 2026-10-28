#include "axis.h"

Axis::Axis(std::shared_ptr<DriverController> driver_controller, std::shared_ptr<IPositionSensor> position_sensor, std::shared_ptr<ILimitersSensor> limiters_sensor) :
    driver_controller_(std::move(driver_controller)),
    position_sensor_(std::move(position_sensor)),
    limiters_sensor_(std::move(limiters_sensor)) {

}

AxisState Axis::state()
{
    return {
        .position = position_sensor_->value().value.angle,
        .turns = position_sensor_->value().value.turns.value(),
        .limiters = limiters_sensor_->value().value,
        .positionValid = position_sensor_->value().valid,
        .limitersValid = limiters_sensor_->value().valid,
        .working_sector = working_sector_
    };
}

void Axis::move(double velocity)
{
    driver_controller_->move(velocity);
}

void Axis::stop()
{
    driver_controller_->stop();
}
