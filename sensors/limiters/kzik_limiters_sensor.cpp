#include "kzik_limiters_sensor.h"

KzikLimitersSensor::KzikLimitersSensor(std::unique_ptr<Pr102Device> device, int pack_offset) :
    device_(std::move(device)),
    pack_offset_(pack_offset)
{
}

Measurement<LimitersState> KzikLimitersSensor::value()
{
    const auto raw = device_->measurement();

    if (!raw.valid) {
        return {{}, raw.timestamp, false};
    }

    const quint16 limiters_mask = raw.value.registers_data[0];

    const auto limiters_state = unpack(limiters_mask);

    return {
        limiters_state,
        raw.timestamp,
        raw.valid
    };
}

LimitersState KzikLimitersSensor::unpack(const quint16 mask) const
{
    return {
        .BO_minus = BitUtils::bitCheck(mask, 0 + pack_offset_),
        .BO_plus = BitUtils::bitCheck(mask, 1 + pack_offset_),
        .HO_minus = BitUtils::bitCheck(mask, 2 + pack_offset_),
        .HO_plus = BitUtils::bitCheck(mask, 3 + pack_offset_),
        .CK_minus = BitUtils::bitCheck(mask, 4 + pack_offset_),
        .CK_plus = BitUtils::bitCheck(mask, 5 + pack_offset_)
    };
}
