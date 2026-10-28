#ifndef CONTROL_SERVICE_H
#define CONTROL_SERVICE_H


#include <core/axis.h>
#include <core/control/modes/i_control_mode.h>
#include <expected>

struct ControlServiceTelemetry {
    AxisState az;
    AxisState el;

};

enum class InputError {
    InvalidDt,
    InvalidAzPosition,
    InvalidElPosition,
    InvalidAzLimiters,
    InvalidElLimiters,
    InvalidSignalPower
};

enum class OutputError {
    InvalidAzVelocity,
    InvalidElVelocity
};

class ControlService
{

public:
    explicit ControlService();

    void startMode(std::unique_ptr<IControlMode> mode);
    void stopMode();
    void update(double dt);
    ControlInput collectTelemetry(double dt);

private:
    std::unique_ptr<IControlMode> active_mode_;
    std::shared_ptr<Axis> axis_az_;
    std::shared_ptr<Axis> axis_el_;

    void apply(const ControlOutput& output);
    std::expected<void, InputError> validate_input(const ControlInput& input, bool requires_signal) const;
    std::expected<ControlOutput, OutputError> limit_out(const ControlInput& input, const ControlOutput& output);

    void stop_drives();
};

#endif // CONTROL_SERVICE_H
