#ifndef DRIVER_CONTROLLER_H
#define DRIVER_CONTROLLER_H

#include <drivers/i_driver.h>
#include <memory>
#include <vector>

class DriverController
{
public:
    explicit DriverController(std::vector<std::unique_ptr<IDriver>> others, std::unique_ptr<IDriver> broadband_driver);

    void move(double velocity);
    void stop();
private:
    std::vector<std::unique_ptr<IDriver>> others_;
    std::unique_ptr<IDriver> broadband_driver_;

};

#endif // DRIVER_CONTROLLER_H
