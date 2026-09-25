#ifndef I_DRIVER_H
#define I_DRIVER_H

enum class MoveDirection {
    FORWARD = 1,
    BACKWARD = -1
};

class IDriver {

public:
    virtual void move(float speed, MoveDirection direction = MoveDirection::FORWARD) const = 0;
    virtual void stop() const = 0;
};

#endif // I_DRIVER_H
