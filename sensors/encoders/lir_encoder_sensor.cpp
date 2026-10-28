#include "lir_encoder_sensor.h"
#include <utilities/angle_utils.h>
#include <iostream>

LirEncoderSensor::LirEncoderSensor(std::unique_ptr<Lir919dDevice> device) :
    device_(std::move(device))
{

}


Measurement<PositionMeasurement> LirEncoderSensor::value() const
{
    const auto raw = device_->measurement();

    if (!raw.valid) {
        return {{}, raw.timestamp, false};
    }

    auto angle = AngleUtils::angle_from_descrete(
        raw.value.encoder_descrete,
        device_->bits_per_turn()
        );

    if (!angle) {
        std::cerr << "LirEncoderSensor: "
                  << AngleUtils::angle_error_message(angle.error()) << '\n';

        return {{}, raw.timestamp, false};
    }

    auto turns = AngleUtils::turns_from_descrete(
        raw.value.encoder_descrete,
        device_->bits_per_turn(),
        device_->turn_count_bits()
        );

    if (!turns) {
        std::cerr << "LirEncoderSensor: "
                  << AngleUtils::angle_error_message(turns.error()) << '\n';

        return {{}, raw.timestamp, false};
    }

    return {
        {*angle, *turns},
        raw.timestamp,
        device_->is_online()
    };
}