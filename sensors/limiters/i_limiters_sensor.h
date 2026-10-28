#ifndef I_LIMITERS_SENSOR_H
#define I_LIMITERS_SENSOR_H

#include <sensors/measurement.h>

struct LimitersState {
    bool BO_minus = false;
    bool BO_plus = false;

    bool HO_minus = false;
    bool HO_plus = false;

    bool CK_minus = false;
    bool CK_plus = false;
};

struct ILimitersSensor {
    virtual ~ILimitersSensor() = default;

    virtual Measurement<LimitersState> value() = 0;
};

#endif // I_LIMITERS_SENSOR_H
