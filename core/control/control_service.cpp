#include "control_service.h"
#include <cmath>

ControlService::ControlService() {}

void ControlService::startMode(std::unique_ptr<IControlMode> mode)
{
    active_mode_ = std::move(mode);
}

void ControlService::stopMode()
{

}

void ControlService::update(double dt)
{
    if (!active_mode_) {
        return;
    }

    auto input = collectTelemetry(dt);

    auto validation = validate_input(
        input,
        active_mode_->requires_signal()
        );

    if (!validation) {
        stop_drives();
        // Передать событие ошибки в FSM
        return;
    }

    auto output = active_mode_->calculate(input);

    auto safe_output = limit_out(input, output);

    if (!safe_output) {
        stop_drives();
        // Передать событие ошибки расчёта в FSM
        return;
    }

    apply(*safe_output);
}

ControlInput ControlService::collectTelemetry(double dt)
{
    ControlInput input;
    for (const Axis& axis : axes_) {
        input.axes.push_back(axis.state());
    }
    input.dt = dt;
    input.signal_power = Measurement<double>{0.0};
    return input;
}

void ControlService::apply(const ControlOutput &output)
{
    for (std::size_t i = 0; i < output.velocities.size(); ++i) {
        axes_[i].move(output.velocities[i]);
    }
}


std::expected<void, InputError>
ControlService::validate_input(
    const ControlInput& input,
    bool requires_signal) const
{
    using namespace std::chrono;

    const auto now = steady_clock::now();

    constexpr auto maxSensorAge = 300ms;
    constexpr auto maxLimitersAge = 300ms;
    constexpr auto maxSignalAge = 500ms;

    // Проверка периода управления
    if (!std::isfinite(input.dt)
        || input.dt <= 0.0
        || input.dt > 0.25)
    {
        return std::unexpected(InputError::InvalidDt);
    }

    auto limitersValid = [&](const AxisState& axis) {
        return isFresh(axis.limiters, now, maxLimitersAge);
    };

    // Положение осей
    for (const auto& axis : input.axes) {
        if (!isFresh(axis.position, now, maxSensorAge)
            || !std::isfinite(axis.position.value.angle)) {
            return std::unexpected(
                InputError::InvalidAxisPosition);
        }
        if (!limitersValid(axis)) {
            return std::unexpected(InputError::InvalidAxisLimiters);
        }
    }

    // Мощность сигнала нужна не каждому алгоритму
    if (requires_signal &&
        (!isFresh(input.signal_power, now, maxSignalAge)
         || !std::isfinite(input.signal_power.value)))
    {
        return std::unexpected(
            InputError::InvalidSignalPower);
    }

    return {};
}

std::expected<ControlOutput, OutputError> ControlService::limit_out(const ControlInput &input, const ControlOutput &output)
{
    // 1. Проверяем результат вычислений
    for (const auto& velocity : output.velocities) {
        if (!std::isfinite(velocity)) {
            return std::unexpected(
                OutputError::InvalidAxisVelocity);
        }
    }

    // 2. Создаём копию управляющего воздействия
    auto safe = output;

    // 3. Проверяем концевики
    for (std::size_t i = 0; i < input.axes.size(); ++i) {
        const auto& limits = input.axes[i].limiters.value;
        if (limits.BO_plus && safe.velocities[i] > 0.0) {
            safe.velocities[i] = 0.0;
        }

        if (limits.BO_minus && safe.velocities[i] < 0.0) {
            safe.velocities[i] = 0.0;
        }
    }

    return safe;
}


void ControlService::stop_drives()
{
    for (auto& axis : axes_) {
        axis.stop();
    }
}
