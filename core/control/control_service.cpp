#include "control_service.h"

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
    return {
        .az = axis_az_->state(),
        .el = axis_el_->state(),
        .signal_power = Measurement<double>{0.0},
        .dt = dt
    };
}

void ControlService::apply(const ControlOutput &output)
{
    axis_az_->move(output.azVelocity);
    axis_el_->move(output.elVelocity);
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

    // Положение азимута
    if (!isFresh(input.az.position, now, maxSensorAge)
        || !std::isfinite(input.az.position.value.angle))
    {
        return std::unexpected(
            InputError::InvalidAzPosition);
    }

    // Положение угла места
    if (!isFresh(input.el.position, now, maxSensorAge)
        || !std::isfinite(input.el.position.value.angle))
    {
        return std::unexpected(
            InputError::InvalidElPosition);
    }

    // Концевики обеих осей
    auto limitersValid = [&](const AxisState& axis) {
        return isFresh(axis.limiters, now, maxLimitersAge);
    };

    if (!limitersValid(input.az))
        return std::unexpected(InputError::InvalidAzLimiters);

    if (!limitersValid(input.el))
        return std::unexpected(InputError::InvalidElLimiters);

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
    if (!std::isfinite(output.azVelocity)) {
        return std::unexpected(
            OutputError::InvalidAzVelocity);
    }

    if (!std::isfinite(output.elVelocity)) {
        return std::unexpected(
            OutputError::InvalidElVelocity);
    }

    // 2. Создаём копию управляющего воздействия
    auto safe = output;

    // 3. Проверяем концевики азимута
    const auto& az_limits = input.az.limiters.value;

    if (az_limits.BO_plus && safe.azVelocity > 0.0) {
        safe.azVelocity = 0.0;
    }

    if (az_limits.BO_minus && safe.azVelocity < 0.0) {
        safe.azVelocity = 0.0;
    }

    // 4. Проверяем концевики угла места
    const auto& el_limits = input.el.limiters.value;

    if (el_limits.BO_plus && safe.elVelocity > 0.0) {
        safe.elVelocity = 0.0;
    }

    if (el_limits.BO_minus && safe.elVelocity < 0.0) {
        safe.elVelocity = 0.0;
    }

    return safe;
}


void ControlService::stop_drives()
{
    axis_az_->stop();
    axis_el_->stop();
}
