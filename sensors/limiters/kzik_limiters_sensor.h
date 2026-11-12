#ifndef KZIK_LIMITERS_SENSOR_H
#define KZIK_LIMITERS_SENSOR_H

#include "i_limiters_sensor.h"
#include "devices/pr102_device.h"

class KzikLimitersSensor : public ILimitersSensor
{
public:
    explicit KzikLimitersSensor(std::unique_ptr<Pr102Device> device, int pack_offset);

    // ILimitersSensor interface
public:
    Measurement<LimitersState> value() override;
private:
    std::unique_ptr<Pr102Device> device_;
    LimitersState unpack(const quint16 registers) const;
    const int pack_offset_ = 0;
    std::unique_ptr<QTimer> timer_;
};

#endif // KZIK_LIMITERS_SENSOR_H
