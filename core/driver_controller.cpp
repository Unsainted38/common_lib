#include "driver_controller.h"

DriverController::DriverController(std::vector<std::unique_ptr<IDriver> > others, std::unique_ptr<IDriver> broadband_driver) :
    others_(std::move(others)),
    broadband_driver_(std::move(broadband_driver)) {
}

void DriverController::move(double velocity)
{
    broadband_driver_->move(velocity);
}

void DriverController::stop()
{
    broadband_driver_->stop();
}
