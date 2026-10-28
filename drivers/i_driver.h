#ifndef I_DRIVER_H
#define I_DRIVER_H

enum class DriverType {
    Single,
    Broadband
};

class IDriver {

public:
    virtual ~IDriver() = default;

    virtual DriverType type() = 0;

    virtual void move(double speed) const = 0;
    virtual void stop() const = 0;
};

#endif // I_DRIVER_H
