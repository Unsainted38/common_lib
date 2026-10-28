#include "vesper_driver.h"

VesperDriver::VesperDriver(std::unique_ptr<VesperEngineDevice> device) :
    device_(std::move(device))
{
    if (device_->deviceAddress() == 0) {
        type_ = DriverType::Broadband;
    }
}

quint8 VesperDriver::device_address() const
{
    return device_->deviceAddress();
}

void VesperDriver::move(double frequency) const
{
    device_->startVesper(frequency);
}

void VesperDriver::stop() const
{
    device_->stopVesper();
}


DriverType VesperDriver::type()
{
    return type_;
}