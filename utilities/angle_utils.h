#ifndef ANGLE_UTILS_H
#define ANGLE_UTILS_H
#include <cstdint>
#include <expected>
#include <cmath>
#include <string_view>

enum class AngleError {
    InvalidBitCount,
    InvalidNormingAngle,
    InvalidAngle,
    InvalidTurnsCount
};

enum class AngleRange {
    Unsigned,
    Signed
};

namespace AngleUtils {
[[nodiscard]]
constexpr std::expected<double, AngleError> angle_from_descrete(uint32_t value, unsigned n) {
    if (n == 0 || n > 32) {
        return std::unexpected(AngleError::InvalidBitCount);
    }
    const uint64_t levels = uint64_t{1} << n;
    const uint32_t raw = value & (levels - 1);
    return static_cast<double>(raw) * 360.0 / levels;
};

[[nodiscard]]
constexpr std::expected<unsigned, AngleError> turns_from_descrete(uint32_t value, unsigned offset, unsigned n) {
    if (n == 0 || n > 12 || offset > 32 - n) {
        return std::unexpected(AngleError::InvalidTurnsCount);
    }

    const uint32_t mask = (uint32_t{1} << n) - 1;

    return static_cast<unsigned>((value >> offset) & mask);
}

template<AngleRange Range = AngleRange::Unsigned>
[[nodiscard]]
std::expected<double, AngleError> normalize_angle(double angle, double norming_angle) {
    if (norming_angle <= 0.0 || !std::isfinite(norming_angle)) {
        return std::unexpected(AngleError::InvalidNormingAngle);
    }

    if (!std::isfinite(angle)) {
        return std::unexpected(AngleError::InvalidAngle);
    }

    angle = std::fmod(angle, norming_angle);

    if constexpr (Range == AngleRange::Unsigned) {
        if (angle < 0.0) {
            angle += norming_angle;
        }
    } else if constexpr (Range == AngleRange::Signed) {
        const double half = norming_angle / 2.0;
        if (angle >= half) {
            angle -= norming_angle;
        } else if (angle < -half) {
            angle += norming_angle;
        }
    }

    return angle;
}

[[nodiscard]]
constexpr std::string_view angle_error_message(AngleError error) noexcept
{
    switch (error) {
    case AngleError::InvalidBitCount:
        return "Invalid bit count: check device config";
    case AngleError::InvalidNormingAngle:
        return "Invalid norming angle";
    case AngleError::InvalidAngle:
        return "Invalid angle";
    case AngleError::InvalidTurnsCount:
        return "Invalid turns count: check device config";
    }

    return "Unknown angle error";
}
};

#endif // ANGLE_UTILS_H
