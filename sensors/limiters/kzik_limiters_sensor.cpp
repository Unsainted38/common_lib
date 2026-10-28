#include "kzik_limiters_sensor.h"

KzikLimitersSensor::KzikLimitersSensor(std::unique_ptr<Pr102Device> device) :
    device_(std::move(device))
{

}

Measurement<LimitersState> KzikLimitersSensor::value()
{

}
