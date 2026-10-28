#ifndef LIR_ENCODER_SENSOR_H
#define LIR_ENCODER_SENSOR_H

#include "i_position_sensor.h"
#include <devices/lir919d_device.h>

class LirEncoderSensor : public IPositionSensor
{
public:
    explicit LirEncoderSensor(std::unique_ptr<Lir919dDevice> device);

    // IPositionSensor interface
public:
    Measurement<PositionMeasurement> value() const override;

private:
    std::unique_ptr<Lir919dDevice> device_;
    Measurement<PositionMeasurement> measurement_;
};

#endif // LIR_ENCODER_SENSOR_H
