#ifndef VESPER_DRIVER_H
#define VESPER_DRIVER_H

#include "i_driver.h"
#include <devices/vesper_engine_device.h>

class VesperDriver : public IDriver
{
public:
    explicit VesperDriver(std::unique_ptr<VesperEngineDevice> device);

    quint8 device_address() const;

    // IDriver interface
public:
    void move(double frequency) const override;
    void stop() const override;
    DriverType type() override;

private:
    std::unique_ptr<VesperEngineDevice> device_;
    DriverType type_ = DriverType::Single;
};

#endif // VESPER_DRIVER_H
