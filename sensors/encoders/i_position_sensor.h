#ifndef I_POSITION_SENSOR_H
#define I_POSITION_SENSOR_H

#include <sensors/measurement.h>

struct PositionRange {
    double min = 0.0;
    double max = 360.0;
};

struct PositionMeasurement {
    double angle = 0.0;
    std::optional<int32_t> turns;
};

struct IPositionSensor {
    virtual ~IPositionSensor() = default;
    [[nodiscard]]
    virtual Measurement<PositionMeasurement> value() const = 0;
};

#endif // I_POSITION_SENSOR_H
