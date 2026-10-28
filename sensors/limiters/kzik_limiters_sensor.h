#ifndef KZIK_LIMITERS_SENSOR_H
#define KZIK_LIMITERS_SENSOR_H

#include "i_limiters_sensor.h"
#include "devices/pr102_device.h"

class KzikLimitersSensor : public ILimitersSensor
{
public:
    explicit KzikLimitersSensor(std::unique_ptr<Pr102Device> device);

    // ILimitersSensor interface
public:
    Measurement<LimitersState> value() override;
private:
    std::unique_ptr<Pr102Device> device_;
    Measurement<LimitersState> measurement_;
};

#endif // KZIK_LIMITERS_SENSOR_H
